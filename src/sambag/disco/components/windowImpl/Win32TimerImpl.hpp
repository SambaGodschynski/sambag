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
	static void startThreads() {}
	//-------------------------------------------------------------------------
	/**
	 * does nothing, only for TimerImpl concept.
	 */
	static void closeThreads() {}

}; // Win32TimerImpl
}}} // namespace(s)

#endif /* SAMBAG_WIN32TIMERIMPL_H */
