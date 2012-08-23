/*
 * Win32MMTimerImpl.hpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32MMTIMERIMPL_H
#define SAMBAG_WIN32MMTIMERIMPL_H

#include <sambag/disco/components/Timer.hpp>

namespace sambag { namespace disco {  namespace components {

//=============================================================================
/** 
  * @class Win32MMTimerImpl.
  */
class Win32MMTimerImpl {
//=============================================================================
public:
protected:
private:
public:
	//-------------------------------------------------------------------------
	static void closeAllTimer();
	//-------------------------------------------------------------------------
	void startTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	void stopTimer(Timer::Ptr tm);
	///////////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------------
	/**
	 * does nothing, only for TimerImpl concept.
	 */
	static void startUpTimer();
	//-------------------------------------------------------------------------
	/**
	 * does nothing, only for TimerImpl concept.
	 */
	static void tearDownTimer();

}; // Win32MMTimerImpl
}}} // namespace(s)

#endif /* SAMBAG_WIN32MMTIMERIMPL_H */
