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
 *    TimerPolicy::Milliseconds;
 *    TimerPolicy::Event;
 *    void addListener(boost::function<void(void*, TimerEvent));
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
    typedef typename TimerPolicy::Milliseconds Milliseconds;
    //-------------------------------------------------------------------------
    typedef typename TimerPolicy::Event TimerEvent;
    //-------------------------------------------------------------------------
    static const Milliseconds ONE_MILLI_IN_NANO;
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
    Milliseconds startTime, duration;
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
    void setDuration(Milliseconds d);
    //-------------------------------------------------------------------------
    void start();
    //-------------------------------------------------------------------------
    void stop();
    //-------------------------------------------------------------------------
    Milliseconds getDuration() const {
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
    //-------------------------------------------------------------------------
    void setRefreshRate(Milliseconds d) {
        TimerPolicy::setDelay(d);
    }
    //-------------------------------------------------------------------------
    Milliseconds getRefreshRate() {
        return TimerPolicy::getDelay();
    }

}; // GenericAnimatior
    
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
const typename GenericAnimator<T,Tm,TW,UP>::Milliseconds
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
        TimerPolicy::addListener(
            boost::bind(&ThisClass::update, this, _1, _2)
        );
    }
    c = endValue - startValue;
    clock.start();
    TimerPolicy::setNumRepetitions(-1);
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
    UpdatePolicy::update(endValue);
    UpdatePolicy::finished(endValue);
}
//-----------------------------------------------------------------------------
template < class T,
class Tm,
template <class> class TW,
template <class> class UP
>
void GenericAnimator<T,Tm,TW,UP>::update(void *, const TimerEvent &ev) {
    Milliseconds t = (Milliseconds)clock.elapsed().wall / ONE_MILLI_IN_NANO;
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
void GenericAnimator<T,Tm,TW,UP>::setDuration(Milliseconds d) {
    duration = d;
}
}}} // namespace(s)


#endif /* SAMBAG_GENERICANIMATOR_H */
