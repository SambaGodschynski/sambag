/*
 * Win32TimerImpl.hpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COCOATIMERIMPL_H
#define SAMBAG_COCOATIMERIMPL_H

#include <sambag/disco/components/Timer.hpp>
#include "cocoaimpl/_CocoaTimer.h"

namespace sambag { namespace disco {  namespace components {

//=============================================================================
/** 
  * @class Win32TimerImpl.
  */
class CocoaTimerImpl : public _CocoaTimer {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef _CocoaTimer Impl;
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
	static void startUpTimer();
	//-------------------------------------------------------------------------
	static void tearDownTimer();

}; // CocoaTimerImpl
}}} // namespace(s)

#endif /* SAMBAG_COCOATIMERIMPL_H */
