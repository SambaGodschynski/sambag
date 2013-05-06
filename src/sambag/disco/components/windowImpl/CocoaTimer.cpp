/*
 * CocoaTimer.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#include "CocoaTimer.hpp"


namespace sambag { namespace disco {  namespace components {
//=============================================================================
// class CocoaTimerImpl 
//=============================================================================
//-----------------------------------------------------------------------------
void CocoaTimerImpl::closeAllTimer() {
     Impl::stopAllTimer();
}
//-----------------------------------------------------------------------------
void CocoaTimerImpl::startTimer(Timer::Ptr tm) {
    Impl::startTimer(tm);
}
//-----------------------------------------------------------------------------
void CocoaTimerImpl::stopTimer(Timer::Ptr tm) {
    Impl::stopTimer(tm);
}
//-----------------------------------------------------------------------------
void CocoaTimerImpl::startUpTimer() {
}
//-----------------------------------------------------------------------------
void CocoaTimerImpl::tearDownTimer() {
}
}}} // namespace(s)

#endif // DISCO_USE_COCOA