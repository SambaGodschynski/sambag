/*
 * NullTimerImpl.hpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_NULLTIMERIMPL_H
#define SAMBAG_NULLTIMERIMPL_H

#include <sambag/disco/components/Timer.hpp>

namespace sambag { namespace disco {  namespace components {

//=============================================================================
/** 
  * @class NullTimerImpl.
  */
class NullTimerImpl {
//=============================================================================
public:
protected:
private:
public:
	//-------------------------------------------------------------------------
	static void closeAllTimer() {}
	//-------------------------------------------------------------------------
	void startTimer(Timer::Ptr tm) {
		tm->timerExpired(); // call at once
	}
	//-------------------------------------------------------------------------
	void stopTimer(Timer::Ptr tm) {}
	///////////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------------
	/**
	 * does nothing, only for TimerImpl concept.
	 */
	static void startThreads() {}
	//-------------------------------------------------------------------------
	/**
	 * does nothing, only for TimerImpl concept.
	 */
	static void joinThreads() {}

}; // NullTimerImpl
}}} // namespace(s)

#endif /* SAMBAG_NULLTIMERIMPL_H */
