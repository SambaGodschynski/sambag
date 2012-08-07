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

namespace sambag { namespace disco {  namespace components {
///////////////////////////////////////////////////////////////////////////////
namespace { // thread stuff
	//-------------------------------------------------------------------------
	boost::thread timerThread;
	//-------------------------------------------------------------------------
	boost::asio::io_service io;
	//-------------------------------------------------------------------------
	bool threadsAreRunning = true;
	//-------------------------------------------------------------------------
	sambag::com::RecursiveMutex timerLock;
	//-------------------------------------------------------------------------
	void timerThreadClbk() { //TODO: impl. 1 thread per timer
		while (threadsAreRunning) {
			io.run();
			io.reset();
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
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		BOOST_FOREACH(ToInvoke::left_map::value_type &v, toInvoke.left) {
			v.first->cancel();
		}
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::startTimer(Timer::Ptr tm) {
	Timer::TimeType ms = tm->getInitialDelay();
	int repetitions = tm->getNumRepetitions();
	BoostTimer *t = new BoostTimer(io, boost::posix_time::millisec(ms));
	toInvoke.insert( ToInvoke::value_type(t, tm) );
	t->async_wait(
		boost::bind(&timerCallback,
		boost::asio::placeholders::error,
		t,
		repetitions)
	);
	tm->__setRunningByToolkit_(true);
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::stopTimer(Timer::Ptr tm) {
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
			!threadsAreRunning ||
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
	threadsAreRunning = true;
	timerThread = boost::thread(&timerThreadClbk);
}
//-----------------------------------------------------------------------------
void BoostTimerImpl::closeThreads() {
	threadsAreRunning = false;
	closeAllTimer();
	timerThread.join();
}

}}} // namespace(s)
