/*
 * Win32TimerImpl.hpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32TIMERIMPL_H
#define SAMBAG_WIN32TIMERIMPL_H

#include <sambag/disco/components/Timer.hpp>

namespace sambag { namespace disco {  namespace components {

//=============================================================================
/** 
  * @class Win32TimerImpl.
  */
class Win32TimerImpl {
//=============================================================================
public:
protected:
	//-------------------------------------------------------------------------
	void startTimerImpl(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	void stopTimerImpl(Timer::Ptr tm);
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
	static void startUpTimer();
	//-------------------------------------------------------------------------
	static void tearDownTimer();

}; // Win32TimerImpl
}}} // namespace(s)

#endif /* SAMBAG_WIN32TIMERIMPL_H */
