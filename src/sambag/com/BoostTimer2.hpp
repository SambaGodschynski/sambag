/*
 * BoostTimer2.hpp
 *
 *  Created on: Sun Jul 14 17:03:28 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BOOSTTIMER2_H
#define SAMBAG_BOOSTTIMER2_H

#include <boost/shared_ptr.hpp>
#include "GenericTimer.hpp"
#include "BoostTimerImpl2.hpp"

namespace sambag { namespace com {

struct BoostTimerIIEvent {
};
struct BoostTimerIIEventPolicy : sambag::com::events::EventSender<BoostTimerIIEvent>
{
    typedef BoostTimerIIEvent Event;
    typedef sambag::com::events::EventSender<BoostTimerIIEvent> Sender;
    template<class _Timer>
    void fireEvent(const _Timer &tm) {
        Sender::notifyListeners(this, BoostTimerIIEvent());
    }
};
struct BoostTimermplPolicy2 : public BoostTimerImpl2 {
    typedef sambag::com::BoostTimerImpl2 Impl;
    template<class _Timer>
    void startTimer( const _Timer &tm ) {
        Impl::startTimer(tm.getPtr());
    }
    template<class _Timer>
    void stopTimer( const _Timer &tm ) {
        Impl::stopTimer(tm.getPtr());
    }
};
typedef sambag::com::GenericTimer
    <BoostTimerIIEventPolicy, BoostTimermplPolicy2> BoostTimer2;

}} // namespace(s)

#endif /* SAMBAG_BOOSTTIMER2_H */
