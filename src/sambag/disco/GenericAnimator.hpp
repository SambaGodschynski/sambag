/*
 * GenericAnimator.hpp
 *
 *  Created on: Wed Jun  5 20:38:15 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GENERICANIMATOR_H
#define SAMBAG_GENERICANIMATOR_H

#include <boost/timer/timer.hpp>
#include <boost/bind.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
template <class T>
struct BasicUpdater {
    void update(const T &val) {}
    void finished(const T &val) {}
};
//=============================================================================
/**
 * @class GenericAnimator.
 *
 * TweenPolicy concept:
 *   T calc(T b, T e, T d, T c, T t);
 * UpdatePolicy concept:
 *   void update(T val);
 *   void finished(const T &val) {}
 *  TimerPolicy:
 *    TimerPolicy::TimeType Millisecond;
 *    TimerPolicy::TimeType TimerEvent;
 *    void addTimerListener(boost::function<void(void*, TimerEvent));
 *    void start();
 *    void stop();
 */
template < class T,
    class _TimerPolicy,
    template <class> class _TweenPolicy,
    template <class> class _UpdatePolicy
>
class GenericAnimator :
    public _TimerPolicy,
    public _TweenPolicy<T>,
    public _UpdatePolicy<T>
{
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef T ValueType;
    //-------------------------------------------------------------------------
    typedef _TweenPolicy<T> TweenPolicy;
    //-------------------------------------------------------------------------
    typedef _UpdatePolicy<T> UpdatePolicy;
    //-------------------------------------------------------------------------
    typedef _TimerPolicy TimerPolicy;
    //-------------------------------------------------------------------------
    typedef GenericAnimator<T, _TimerPolicy, _TweenPolicy, _UpdatePolicy> ThisClass;
    //-------------------------------------------------------------------------
    typedef typename TimerPolicy::TimeType Millisecond;
    //-------------------------------------------------------------------------
    typedef typename TimerPolicy::TimerEvent TimerEvent;
    //-------------------------------------------------------------------------
    static const Millisecond ONE_MILLI_IN_NANO;
protected:
    //-------------------------------------------------------------------------
    GenericAnimator() : firstInit(true), duration(0) {}
    //-------------------------------------------------------------------------
    void update(void *, const TimerEvent &ev);
private:
    //-------------------------------------------------------------------------
    bool firstInit;
    //-------------------------------------------------------------------------
    typedef boost::timer::cpu_timer Clock;
    //-------------------------------------------------------------------------
    Clock clock;
    //-------------------------------------------------------------------------
    T startValue, endValue, c, current;
    //-------------------------------------------------------------------------
    Millisecond startTime, duration;
public:
    //-------------------------------------------------------------------------
    const T & getCurrentValue() const {
        return current;
    }
    //-------------------------------------------------------------------------
    void setStartValue(const T &val);
    //-------------------------------------------------------------------------
    void setEndValue(const T &val);
    //-------------------------------------------------------------------------
    void setDuration(Millisecond d);
    //-------------------------------------------------------------------------
    void start();
    //-------------------------------------------------------------------------
    void stop();
    //-------------------------------------------------------------------------
    Millisecond getDuration() const {
        return duration;
    }
    //-------------------------------------------------------------------------
    const T & getStartValue() const {
        return startValue;
    }
    //-------------------------------------------------------------------------
    const T & getEndValue() const {
        return endValue;
    }
}; // GenericAnimatior
    
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
const typename GenericAnimator<T,Tm,TW,UP>::Millisecond
    GenericAnimator<T,Tm,TW,UP>::ONE_MILLI_IN_NANO = 1000000;
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::start() {
    if (firstInit) {
        firstInit=false;
        TimerPolicy::addEventListener(
            boost::bind(&ThisClass::update, this, _1, _2)
        );
    }
    c = endValue - startValue;
    clock.start();
    TimerPolicy::start();
}
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::stop() {
    // stop timer
    TimerPolicy::stop();
    // reset clock
    clock.stop();
    clock.start();
    clock.stop();
    UpdatePolicy::finished(current);
}
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::update(void *, const TimerEvent &ev) {
    Millisecond t = (Millisecond)clock.elapsed().wall / ONE_MILLI_IN_NANO;
    if (t>=duration) {
        stop();
        current = endValue;
        UpdatePolicy::finished(current);
    } else {
        current = TweenPolicy::calc(startValue, duration, c, t);
    }
    UpdatePolicy::update(current);
}
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::setStartValue(const T &val) {
    startValue = val;
}
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::setEndValue(const T &val) {
    endValue = val;
    c = endValue - startValue;
}
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::setDuration(Millisecond d) {
    duration = d;
}
}}} // namespace(s)


#endif /* SAMBAG_GENERICANIMATOR_H */
