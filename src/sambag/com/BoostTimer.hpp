/*
 * BoostTimer.hpp
 *
 *  Created on: Sun Jul 14 16:19:09 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BOOSTTIMER_H
#define SAMBAG_BOOSTTIMER_H

#include <boost/shared_ptr.hpp>
#include <sambag/com/GenericTimer.hpp>
#include <sambag/com/BoostTimerImpl.hpp>

namespace sambag { namespace com {

struct BoostTimerEvent {
};
struct BoostTimerEventPolicy : sambag::com::events::EventSender<BoostTimerEvent>
{
    typedef BoostTimerEvent Event;
    typedef sambag::com::events::EventSender<BoostTimerEvent> Sender;
    template<class _Timer>
    void fireEvent(const _Timer &tm) {
        Sender::notifyListeners(this, BoostTimerEvent());
    }
};
struct BoostTimerImplPolicy {
    typedef sambag::com::BoostTimerImpl Impl;
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
    <BoostTimerEventPolicy, BoostTimerImplPolicy> BoostTimer;






}} // namespace(s)

#endif /* SAMBAG_BOOSTTIMER_H */
