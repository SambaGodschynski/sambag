/*
 * BoostTimerImpl2.cpp
 *
 *  Created on: Sun Jul 14 16:35:17 2013
 *      Author: Johannes Unger
 */

#include "BoostTimerImpl2.hpp"
#include <sambag/com/Thread.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_set.hpp>

namespace {

struct WorkerThread {
    typedef boost::thread Thread;
    typedef boost::asio::io_service Io;
    typedef boost::asio::io_service::work Work;
    typedef boost::shared_ptr<Work> WorkPtr;
    Thread th;
    Io io;
    WorkPtr work;
    WorkerThread();
    ~WorkerThread();
};
WorkerThread::WorkerThread() {
    work = WorkPtr( new Work(io) );
    th = Thread(boost::bind(&boost::asio::io_service::run, &io));
}

WorkerThread::~WorkerThread() {  
	work.reset();
    io.stop();
	if (th.try_join_for(boost::chrono::seconds(1))) {
		return;
	}
	SAMBAG_LOG_WARN<<"WorkerThread does not close";
    th.join();
}

sambag::com::RecursiveMutex __mutex;
typedef boost::shared_ptr<WorkerThread> WkPtr;
typedef boost::weak_ptr<WorkerThread> WkWPtr;

WkWPtr __wkt;
WkPtr __wktHolder;

WorkerThread * __getWkt() {
	WkPtr res = __wkt.lock();
    if (!res) {
		__wkt = res = __wktHolder = WkPtr( new WorkerThread() );
    }
    return res.get();
}

void timerExpired(const boost::system::error_code& error,
sambag::com::ITimer::WPtr _tm,
sambag::com::BoostTimerImpl2::Timer *timerImpl)
{
    if (error == boost::asio::error::operation_aborted) {
        return;
    }
    sambag::com::ITimer::Ptr tm = _tm.lock();
    if (tm) {
        if (!tm->isRunning()) {
            return;
        }
        tm->timerExpired();
        int &i = tm->__getNumCalled_();
        ++i;
        if (i>=tm->getNumRepetitions() && tm->getNumRepetitions()>=0) {
            return;
        }
        sambag::com::ITimer::Milliseconds ms = tm->getDelay();
        timerImpl->expires_from_now(boost::posix_time::milliseconds(ms));
        timerImpl->async_wait( boost::bind(&timerExpired, _1, _tm, timerImpl) );
    }
}
} // namespace(s)

namespace sambag {  namespace com {
namespace {
	typedef boost::unordered_set<BoostTimerImpl2*> _RunningTimer;
	_RunningTimer _runningTimer;
}
//=============================================================================
//  Class BoostTimerImpl2
//=============================================================================
//-----------------------------------------------------------------------------
BoostTimerImpl2::WorkerThreadHolder BoostTimerImpl2::startWorkerThread() {
	__getWkt();
	WorkerThreadHolder res = __wktHolder;
	__wktHolder.reset();
	return res;
}
//-----------------------------------------------------------------------------
BoostTimerImpl2::BoostTimerImpl2() {
	_runningTimer.insert( this );
}
//-----------------------------------------------------------------------------
void BoostTimerImpl2::startTimer(ITimer::Ptr tm) {
	if (!timer) {
		timer = TimerPtr( new Timer(__getWkt()->io) );
	}
    ITimer::Milliseconds ms = tm->getInitialDelay();
    tm->__getNumCalled_() = 0;
    timer->expires_from_now(boost::posix_time::milliseconds(ms));
    timer->async_wait( boost::bind(&timerExpired, _1, ITimer::WPtr(tm), timer.get()) );
}
//-----------------------------------------------------------------------------
void BoostTimerImpl2::stopTimer(ITimer::Ptr tm) {
	if (timer) {
		timer->cancel();
	}
}
//-----------------------------------------------------------------------------
BoostTimerImpl2::~BoostTimerImpl2() {
	stopTimer(ITimer::Ptr());
	timer.reset();

	_RunningTimer::iterator it = _runningTimer.find(this);
	if (it!=_runningTimer.end()) {
		_runningTimer.erase(it);
	}
}
//-----------------------------------------------------------------------------
void BoostTimerImpl2::closeAllTimer() {
	_RunningTimer::iterator it = _runningTimer.begin();
	for (; it!=_runningTimer.end(); ++it) {
		(*it)->stopTimer();
		(*it)->timer.reset();
	}
}
}} // namespace(s)
