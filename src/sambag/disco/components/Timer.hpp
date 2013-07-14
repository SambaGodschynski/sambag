/*
 * Timer.hpp
 *
 *  Created on: Mon Jun 25 10:35:04 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TIMER_H
#define SAMBAG_TIMER_H

#include <boost/shared_ptr.hpp>
#include "events/ActionEvent.hpp"
#include <sambag/com/events/Events.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/com/GenericTimer.hpp>
#include <boost/static_assert.hpp>

namespace sambag { namespace disco { namespace components {
namespace ev = sambag::com::events;
//=============================================================================
struct TimerEvent {
	sambag::com::ITimer::Ptr src;
	TimerEvent(sambag::com::ITimer::Ptr src) : src(src) {}
	sambag::com::ITimer::Ptr getSource() const { return src; }

};
struct TookitTimerEventPolicy :
    public sambag::com::events::EventSender<TimerEvent>
{
    typedef TimerEvent Event;
    template <class _Timer>
    void fireEvent(const _Timer &ev) {
        sambag::com::events::EventSender<TimerEvent>::notifyListeners(
            this,
            TimerEvent(ev.getPtr())
        );
    }
};

template <class _Timer>
void __startTimer(const _Timer &tm) {
    BOOST_STATIC_ASSERT(sizeof(_Timer) == 0);
}

template <class _Timer>
void __stopTimer(const _Timer &tm) {
    BOOST_STATIC_ASSERT(sizeof(_Timer) == 0);
}

class ToolkitTimer {
private:
public:
    template <class _Timer>
    void startTimer(const _Timer &t) {
        __startTimer(t);
    }
    template <class _Timer>
    void stopTimer(const _Timer &t) {
        __stopTimer(t);
    }
};

typedef sambag::com::GenericTimer<TookitTimerEventPolicy, ToolkitTimer> Timer;
template <>
void __startTimer<Timer>(const Timer &tm);
template <>
void __stopTimer<Timer>(const Timer &tm);
}}} // namespace(s)

#endif /* SAMBAG_TIMER_H */
