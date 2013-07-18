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
private:
    Timer timer;
public:
    //-------------------------------------------------------------------------
    BoostTimerImpl2();
	//-------------------------------------------------------------------------
	void startTimer(ITimer::Ptr tm);
	//-------------------------------------------------------------------------
	void stopTimer(ITimer::Ptr tm);
    //-------------------------------------------------------------------------
    virtual ~BoostTimerImpl2();
	//-------------------------------------------------------------------------
	static void closeAllTimer();
}; // BoostTimerImpl2
}} // namespace(s)

#endif /* SAMBAG_BOOSTTIMERIMPL2_H */
