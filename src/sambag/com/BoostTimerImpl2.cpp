/*
 * BoostTimerImpl2.cpp
 *
 *  Created on: Sun Jul 14 16:35:17 2013
 *      Author: Johannes Unger
 */

#include "BoostTimerImpl2.hpp"
#include <sambag/com/Thread.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>

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
    th.join();
}

sambag::com::RecursiveMutex __mutex;
typedef boost::shared_ptr<WorkerThread> WkPtr;
WkPtr __wkt;

WorkerThread * __getWkt() {
    if (!__wkt) {
            __wkt = WkPtr( new WorkerThread() );
    }
    return __wkt.get();
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
//=============================================================================
//  Class BoostTimerImpl2
//=============================================================================
//-----------------------------------------------------------------------------
bool BoostTimerImpl2::startWorkerThread() {
    if (__wkt) {
        return false;
    }
    __getWkt();
    return true;
}
//-----------------------------------------------------------------------------
BoostTimerImpl2::BoostTimerImpl2() : timer(__getWkt()->io) {
}
//-----------------------------------------------------------------------------
void BoostTimerImpl2::startTimer(ITimer::Ptr tm) {
    ITimer::Milliseconds ms = tm->getInitialDelay();
    tm->__getNumCalled_() = 0;
    timer.expires_from_now(boost::posix_time::milliseconds(ms));
    timer.async_wait( boost::bind(&timerExpired, _1, ITimer::WPtr(tm), &timer) );
}
//-----------------------------------------------------------------------------
void BoostTimerImpl2::stopTimer(ITimer::Ptr tm) {
    timer.cancel();
}
//-----------------------------------------------------------------------------
BoostTimerImpl2::~BoostTimerImpl2() {
}
//-------------------------------------------------------------------------
void BoostTimerImpl2::closeAllTimer() {
}
}} // namespace(s)
