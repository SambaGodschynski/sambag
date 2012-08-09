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
		StartThread,
		CloseThread
	};
	//-------------------------------------------------------------------------
	typedef int Id;
	//-------------------------------------------------------------------------
	typedef std::pair<Messages, Id> Message;
	//-------------------------------------------------------------------------
	typedef std::queue<Message> MessageQueue;
	//-------------------------------------------------------------------------
	MessageQueue msgQueue;
	//-------------------------------------------------------------------------
	sambag::com::RecursiveMutex queueLock;
	//-------------------------------------------------------------------------
	typedef boost::thread MainThread;
	//-------------------------------------------------------------------------
	typedef std::vector<TimerThread*> TimerThreads;
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
struct TimerThread {
//=============================================================================
	Timer::Ptr tm;
	boost::thread *thread;
	boost::asio::io_service *io;
	bool isRunning;
	bool sleep;
	Id id;
	TimerThread( Timer::Ptr tm = Timer::Ptr(), Id id = -1 ) : 
		tm(tm), 
		thread(NULL),
		io(NULL), 
		isRunning(false),
		sleep(false)
		id(id) {}
	~TimerThread() {
		if (thread)
			delete thread;
		if (io)
			delete io;
	}
	void start();
	void stop();
	void timerThreadClbk();
};
//-----------------------------------------------------------------------------
void TimerThread::start() {
	io = new boost::asio::io_service();
	// prepare timer
	Timer::TimeType ms = tm->getInitialDelay();
	int repetitions = tm->getNumRepetitions();
	BoostTimerImpl::BoostTimer *t = 
		new BoostTimerImpl::BoostTimer(*io, boost::posix_time::millisec(ms));
	BoostTimerImpl::toInvoke.insert( BoostTimerImpl::ToInvoke::value_type(t, tm) );
	t->async_wait(
		boost::bind(&BoostTimerImpl::timerCallback,
		boost::asio::placeholders::error,
		t,
		repetitions)
	);
	tm->__setRunningByToolkit_(true);
	// start timer thread
	thread = new boost::thread(
		boost::bind(&TimerThread::timerThreadClbk, this)
	);
}
//-----------------------------------------------------------------------------
void TimerThread::stop() {
	isRunning = false;
	thread->join();
}
//-----------------------------------------------------------------------------
void TimerThread::timerThreadClbk() {
	io->run();
	SAMBAG_BEGIN_SYNCHRONIZED(queueLock)
		msgQueue.push(Message(CloseThread, id));
	SAMBAG_END_SYNCHRONIZED
	while (isRunning) { // wait for exiting by main thread
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
}
///////////////////////////////////////////////////////////////////////////////
namespace {
	//-------------------------------------------------------------------------
	void handleMessage(const Message &msg) {
		Id id = msg.second;
		TimerThread &th = *timerThreads[id];
		switch(msg.first) {
			case StartThread:
				th.start();
				break;
			case CloseThread:
				th.stop();
				break;
		}
	}
	//-------------------------------------------------------------------------
	void mainThreadClbk() {
		while (mainThreadRunning) {
			SAMBAG_BEGIN_SYNCHRONIZED(queueLock)
				while( !msgQueue.empty() ) {
					Message msg = msgQueue.front();
					handleMessage(msg);
					msgQueue.pop();
				}
			SAMBAG_END_SYNCHRONIZED
			boost::this_thread::sleep(boost::posix_time::millisec(10));
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
		SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
			BOOST_FOREACH(ToInvoke::left_map::value_type &v, toInvoke.left) {
				v.first->cancel();
			}
		SAMBAG_END_SYNCHRONIZED
	} catch (...) {
		// one timer is locked	
	}
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::startTimer(Timer::Ptr tm) {
	if (tm->isRunning())
		return;
	Id id = timerThreads.size();
	timerThreads.push_back(new TimerThread(tm, id));
	// invoke: add thread
	SAMBAG_BEGIN_SYNCHRONIZED(queueLock)
		msgQueue.push(Message(StartThread, id));
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::stopTimer(Timer::Ptr tm) {
	if (!tm->isRunning())
		return;
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		ToInvoke::right_map::iterator it = toInvoke.right.find(tm);
		if (it==toInvoke.right.end()) // timerImpl not found
			return;
		BoostTimer *timerImpl = it->second;
		tm->__setRunningByToolkit_(false);
		timerImpl->cancel();
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::timerCallback(const boost::system::error_code&,
		BoostTimerImpl::BoostTimer *timerImpl, int repetitions)
{
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		ToInvoke::left_map::iterator it = toInvoke.left.find(timerImpl);
		if (it==toInvoke.left.end())
			return;
		Timer::Ptr tm = it->second;
		tm->timerExpired();
		if (repetitions == 0 ||
			!mainThreadRunning ||
			!tm->isRunning()) // stop forced
		{
			tm->__setRunningByToolkit_(false);
			toInvoke.left.erase(it);
			delete timerImpl;
			return;
		}
		if (repetitions > 0)
			--repetitions;
		long ms = tm->getDelay();
		timerImpl->expires_at(timerImpl->expires_at() +
				boost::posix_time::millisec(ms));
		timerImpl->async_wait(
			boost::bind(&timerCallback,
			boost::asio::placeholders::error,
			timerImpl,
			repetitions)
		);
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::startThreads() {
	mainThreadRunning = true;
	mainThread = boost::thread(&mainThreadClbk);
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::joinThreads() {
	mainThreadRunning = false;
	closeAllTimer();
	mainThread.join();
	for (Id id = 0; id<timerThreads.size(); ++id) {
		delete timerThreads[id];
	}
}
//-----------------------------------------------------------------------------
int BoostTimerImpl::getMaxNumThreads() {
	return timerThreads.size();
}

}}} // namespace(s)
