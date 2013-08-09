/*
 * BoostTimerImpl2.hpp
 *
 *  Created on: Sun Jul 14 16:35:17 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BOOSTTIMERIMPL2_H
#define SAMBAG_BOOSTTIMERIMPL2_H

#include <boost/shared_ptr.hpp>
#include "ITimer.hpp"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/high_resolution_timer.hpp>

namespace sambag { namespace com {
//=============================================================================
/** 
  * @class BoostTimerImpl2.
  * Timer impl. using boost timer, asio::io.
  */
class BoostTimerImpl2 {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef boost::asio::deadline_timer Timer;
	typedef boost::shared_ptr<Timer> TimerPtr;
private:
    TimerPtr timer;
public:
    //-------------------------------------------------------------------------
    /**
     * @return shared pointer to the worker thread.
	 * If the shared pointer will be released the thread will be closed.
     * @note calling startWorkerThread is optional. It exists to have the
     * possibility to set the thread start and end timepoint manually 
	 * (call it before starting a timer)
     * Otherwise the thread starts on first need. 
     */
	typedef boost::shared_ptr<void> WorkerThreadHolder;
    static WorkerThreadHolder startWorkerThread();
    //-------------------------------------------------------------------------
    BoostTimerImpl2();
	//-------------------------------------------------------------------------
	void startTimer(ITimer::Ptr tm);
	//-------------------------------------------------------------------------
	/**
	 * @param not needed only for concept purpose
	 */
	void stopTimer(ITimer::Ptr tm=ITimer::Ptr());
    //-------------------------------------------------------------------------
    virtual ~BoostTimerImpl2();
	//-------------------------------------------------------------------------
	static void closeAllTimer();
}; // BoostTimerImpl2
}} // namespace(s)

#endif /* SAMBAG_BOOSTTIMERIMPL2_H */
