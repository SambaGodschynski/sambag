/*
 * BoostTimerImpl.cpp
 *
 *  Created on: Tue Aug  7 09:41:30 2012
 *      Author: Johannes Unger
 */

#include "BoostTimerImpl.hpp"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sambag/com/Thread.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <queue>

namespace sambag { namespace disco {  namespace components {
///////////////////////////////////////////////////////////////////////////////
namespace { // thread stuff
	//-------------------------------------------------------------------------
	enum Messages {
		StartTimer,
		CloseThread
	};
	//-------------------------------------------------------------------------
	enum {SLEEPING_TIME = 10}; // for timer and main thread
	//-------------------------------------------------------------------------
	typedef std::pair<Messages, boost::shared_ptr<TimerThread> > Message;
	//-------------------------------------------------------------------------
	typedef std::queue<Message> MessageQueue;
	//-------------------------------------------------------------------------
	MessageQueue msgQueue;
	//-------------------------------------------------------------------------
	sambag::com::RecursiveMutex msgQueueLock;
	//-------------------------------------------------------------------------
	typedef boost::thread MainThread;
	//-------------------------------------------------------------------------
	typedef std::vector< boost::shared_ptr<TimerThread> > TimerThreads;
	//-------------------------------------------------------------------------
	typedef std::queue< boost::shared_ptr<TimerThread> > FreeTimerThreads;
	//-------------------------------------------------------------------------
	MainThread mainThread;
	//-------------------------------------------------------------------------
	TimerThreads timerThreads;
	//-------------------------------------------------------------------------
	sambag::com::RecursiveMutex timerLock;
	//-------------------------------------------------------------------------
	bool mainThreadRunning = true;
} // namespace
//=============================================================================
class TimerThread {
//=============================================================================
public:
	typedef boost::shared_ptr<TimerThread> Ptr;
	typedef boost::weak_ptr<TimerThread> WPtr;
private:
	static FreeTimerThreads freeThreads;
	static sambag::com::RecursiveMutex freeTimerLock;
	Timer::Ptr tm;
	boost::thread *thread;
	boost::asio::io_service *io;
	bool threadIsRunning;
	bool timerIsRunning;
protected:
	WPtr self;
	TimerThread( Timer::Ptr tm = Timer::Ptr() ) : 
		tm(tm), 
		thread(NULL),
		io(NULL), 
		threadIsRunning(false),
		timerIsRunning(false)
	{
	}
	void markAsFree();
public:
	void startTimer();
	void startThread();
	void stopThread();
	void timerThreadClbk();
	~TimerThread() {
		if (thread)
			delete thread;
		if (io)
			delete io;
	}
	bool isSleeping() const {
		return !timerIsRunning && threadIsRunning;
	}
	static Ptr get(Timer::Ptr tm = Timer::Ptr());
	Ptr getPtr() const {
		return self.lock();
	}
};
///////////////////////////////////////////////////////////////////////////////
// TimerThread impl.
//-----------------------------------------------------------------------------
FreeTimerThreads TimerThread::freeThreads;
sambag::com::RecursiveMutex TimerThread::freeTimerLock;
//-----------------------------------------------------------------------------
TimerThread::Ptr TimerThread::get(Timer::Ptr tm) {
	Ptr tmth;
	// look for free thread
	SAMBAG_BEGIN_SYNCHRONIZED(freeTimerLock)
		if (!freeThreads.empty()) {
			tmth = freeThreads.front();
			freeThreads.pop();
			tmth->tm = tm;
			return tmth;
		}
	SAMBAG_END_SYNCHRONIZED
	tmth = Ptr ( new TimerThread(tm) );
	tmth->self = tmth;
	timerThreads.push_back(tmth);
	return tmth;
}
//-----------------------------------------------------------------------------
void TimerThread::markAsFree() {
	SAMBAG_BEGIN_SYNCHRONIZED(freeTimerLock)
		freeThreads.push(getPtr());
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void TimerThread::startTimer() {
	if (!io)
		io = new boost::asio::io_service();
	// prepare timer
	Timer::TimeType ms = tm->getInitialDelay();
	BoostTimerImpl::BoostTimer *t = 
		new BoostTimerImpl::BoostTimer(*io, boost::posix_time::millisec(ms));
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		BoostTimerImpl::toInvoke.insert( BoostTimerImpl::ToInvoke::value_type(t, tm) );
	SAMBAG_END_SYNCHRONIZED
	t->async_wait(
		boost::bind(&BoostTimerImpl::timerCallback,
		boost::asio::placeholders::error,
		t)
	);
	tm->__setRunningByToolkit_(true);
	timerIsRunning = true;
	if (!threadIsRunning)
		startThread();
}
//-----------------------------------------------------------------------------
void TimerThread::startThread() {
	if (threadIsRunning)
		return;
	threadIsRunning = true;
	if (!thread) {
		thread = new boost::thread (
			boost::bind(&TimerThread::timerThreadClbk, this)
		);
	}
}
//-----------------------------------------------------------------------------
void TimerThread::stopThread() {
	threadIsRunning = false;
	thread->join();
}
//-----------------------------------------------------------------------------
void TimerThread::timerThreadClbk() {
	while (threadIsRunning) {
		while(timerIsRunning && threadIsRunning) {
			io->run();
			io->reset();
			timerIsRunning = false;
			tm.reset();
			markAsFree();
		}
		while (!timerIsRunning && threadIsRunning) { // wait for exiting by main thread
			boost::this_thread::sleep(boost::posix_time::millisec(SLEEPING_TIME));
			// TODO: close thread after X seconds
			//SAMBAG_BEGIN_SYNCHRONIZED(msgQueueLock)
			//	msgQueue.push(Message(CloseThread, id));
			//SAMBAG_END_SYNCHRONIZED
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
// MainThread impl.
namespace {
	//-------------------------------------------------------------------------
	void handleMessage(const Message &msg) {
		TimerThread::Ptr th = msg.second;
		switch(msg.first) {
			case StartTimer:
				th->startTimer();
				break;
			case CloseThread:
				th->stopThread();
				break;
		}
	}
	//-------------------------------------------------------------------------
	void mainThreadClbk() {
		while (mainThreadRunning) {
			SAMBAG_BEGIN_SYNCHRONIZED(msgQueueLock)
				while( !msgQueue.empty() ) {
					Message msg = msgQueue.front();
					handleMessage(msg);
					msgQueue.pop();
				}
			SAMBAG_END_SYNCHRONIZED
			boost::this_thread::sleep(boost::posix_time::millisec(SLEEPING_TIME));
		}
		// close all timer threads:
		for (size_t i=0; i<timerThreads.size(); ++i) {
			timerThreads[i]->stopThread();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
//  Class BoostTimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
BoostTimerImpl::ToInvoke BoostTimerImpl::toInvoke;
//-----------------------------------------------------------------------------
void BoostTimerImpl::closeAllTimer() {
	try {
		BOOST_FOREACH(ToInvoke::left_map::value_type &v, toInvoke.left) {
			if (!v.first) {
				continue;
			}
			v.first->cancel();
		}
	} catch (...) {
		throw TimerLockedEx();
	}
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::startTimer(Timer::Ptr tm) {
	if (tm->isRunning())
		return;
	TimerThread::Ptr tmth = TimerThread::get(tm);
	// invoke: add thread
	SAMBAG_BEGIN_SYNCHRONIZED(msgQueueLock)
		msgQueue.push(Message(StartTimer, tmth));
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::stopTimer(Timer::Ptr tm) {
	if (!tm->isRunning())
		return;
	ToInvoke::right_map::iterator it = toInvoke.right.find(tm);
	if (it==toInvoke.right.end()) // timerImpl not found
			return;
	BoostTimer *timerImpl = it->second;
	tm->__setRunningByToolkit_(false);
	timerImpl->cancel();
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::timerCallback(const boost::system::error_code&,
		BoostTimerImpl::BoostTimer *timerImpl)
{
		ToInvoke::left_map::iterator it = toInvoke.left.find(timerImpl);
		if (it==toInvoke.left.end())
			return;
		Timer::Ptr tm = it->second;
		int &numCalled = tm->__getNumCalled_();
		++numCalled;
		tm->timerExpired();
		if ( ( tm->getNumRepetitions() != -1 && 
			  numCalled > tm->getNumRepetitions() ) ||
			!mainThreadRunning ||
			!tm->isRunning()) // stop forced
		{
				tm->__setRunningByToolkit_(false);
			SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
				toInvoke.left.erase(it);
				delete timerImpl;
			SAMBAG_END_SYNCHRONIZED
			return;
		}
		long ms = tm->getDelay();
		timerImpl->expires_at(timerImpl->expires_at() +
				boost::posix_time::millisec(ms));
		timerImpl->async_wait(
			boost::bind(&timerCallback,
			boost::asio::placeholders::error,
			timerImpl)
		);
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::startUpTimer() {
	mainThreadRunning = true;
	mainThread = boost::thread(&mainThreadClbk);
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::tearDownTimer() {
	mainThreadRunning = false;
	closeAllTimer();
	mainThread.join();
}
//-----------------------------------------------------------------------------
int BoostTimerImpl::getMaxNumThreads() {
	return timerThreads.size();
}

}}} // namespace(s)
