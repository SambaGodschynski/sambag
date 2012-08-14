/*
 * BoostTimerImpl.hpp
 *
 *  Created on: Tue Aug  7 09:41:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BOOSTTIMERIMPL_H
#define SAMBAG_BOOSTTIMERIMPL_H

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sambag/com/Thread.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <sambag/disco/components/Timer.hpp>

namespace sambag { namespace disco {  namespace components {
class TimerThread;
//=============================================================================
/** 
  * @class BoostTimerImpl.
  */
class BoostTimerImpl {
friend class TimerThread;
//=============================================================================
public:
	//-------------------------------------------------------------------------
	struct TimerLockedEx {};
private:
	//-------------------------------------------------------------------------
	typedef boost::asio::deadline_timer BoostTimer;
	//-------------------------------------------------------------------------
	typedef boost::bimap<
			boost::bimaps::unordered_set_of<BoostTimer*>,
			boost::bimaps::unordered_set_of<Timer::Ptr> > ToInvoke;
	//-------------------------------------------------------------------------
	static void timerCallback(const boost::system::error_code&,
				BoostTimer* timer);
	//-------------------------------------------------------------------------
	static ToInvoke toInvoke;
public:
	//-------------------------------------------------------------------------
	static int getMaxNumThreads();
	//-------------------------------------------------------------------------
	static void closeAllTimer();
	//-------------------------------------------------------------------------
	void startTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	void stopTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	static void startUpTimer();
	//-------------------------------------------------------------------------
	static void tearDownTimer();
}; // BoostTimerImpl
}}} // namespace(s)

#endif /* SAMBAG_BOOSTTIMERIMPL_H */
