/*
 * Animation.cpp
 *
 *  Created on: Wed Feb 13 14:14:30 2013
 *      Author: Johannes Unger
 */

#include "Animation.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class DefaultTimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
void DefaultTimerImpl::addTimerListener(const EventFunction &f) {
    com::events::EventSender<TimerEvent>::addEventListener(f);
}
//-----------------------------------------------------------------------------
void DefaultTimerImpl::start() {
    Super::setNumRepetitions(-1);
    Super::start();
}
//-----------------------------------------------------------------------------
void DefaultTimerImpl::stop() {
    // stop timer
    Super::stop();
}
//-----------------------------------------------------------------------------
void DefaultTimerImpl::setRefreshRate(Millisecond d) {
    Super::setDelay((TimeType)d);
}
}}} // namespace(s)
