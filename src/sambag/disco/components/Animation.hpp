/*
 * Animation.hpp
 *
 *  Created on: Wed Feb 13 14:14:30 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ANIMATON_H
#define SAMBAG_ANIMATON_H

#include <boost/shared_ptr.hpp>
#include "Timer.hpp"
#include <sambag/com/Common.hpp>
#include <boost/timer/timer.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Animation.
  *
  * TweenPolicy concept:
  *   T calc(T b, T e, T d, T c, T t);
  * UpdatePolicy concept:
  *   void update(T val);
  */
template < class T,
	template <class> class _TweenPolicy,
	template <class> class _UpdatePolicy
>
class Animation : public Timer, 
	public _TweenPolicy<T>,
	public _UpdatePolicy<T>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef Timer Super;
	//-------------------------------------------------------------------------
	typedef TimeType Millisecond;
	//-------------------------------------------------------------------------
	typedef T ValueType;
	//-------------------------------------------------------------------------
	typedef _TweenPolicy<T> TweenPolicy;
	//-------------------------------------------------------------------------
	typedef _UpdatePolicy<T> UpdatePolicy;
	//-------------------------------------------------------------------------
	typedef Animation<T, _TweenPolicy, _UpdatePolicy> ThisClass;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ThisClass> Ptr;
	//-------------------------------------------------------------------------
	static const Millisecond ONE_MILLI_IN_NANO;
protected:
	//-------------------------------------------------------------------------
	Animation() : duration(0.) {}
	//-------------------------------------------------------------------------
	void update(void *, const TimerEvent &ev);
private:
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
	void setRefreshRate(Millisecond d);
	//-------------------------------------------------------------------------
	Millisecond getDuration() const {
		return duration;
	}
	//-------------------------------------------------------------------------
	Millisecond getRefreshRate() const {
		return (Millisecond)Super::getDelay();
	}
	//-------------------------------------------------------------------------
	const T & getStartValue() const {
		return startValue;
	}
	//-------------------------------------------------------------------------
	const T & getEndValue() const {
		return endvalue;
	}
	//-------------------------------------------------------------------------
	static Ptr create(const T &s = T(), 
		const T &e = T(), 
		Millisecond d = 0, 
		Millisecond rfsh = 0
	);
	//-------------------------------------------------------------------------
	/**
	 * @overide
	 * starts animation
	 */
	virtual void start();
	//-------------------------------------------------------------------------
	/**
	 * @overide
	 * stops animation
	 */
	virtual void stop();
	//-------------------------------------------------------------------------
	/**
	 * @overide
	 * has no effect
	 */
	virtual void setNumRepetitions(int numRepeats){}
}; // Animation
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
const typename Animation<T,TW,UP>::Millisecond 
Animation<T,TW,UP>::ONE_MILLI_IN_NANO = 1000000;
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::start() {
	if (isRunning()) {
		return;
	}
	c = endValue - startValue;
	clock.start();
	Super::setNumRepetitions(-1);
	Super::start();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::stop() {
	if (!isRunning()) {
		return;
	}
	clock.stop();
	Super::stop();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::update(void *, const TimerEvent &ev) {
	Millisecond t = clock.elapsed().wall / ONE_MILLI_IN_NANO;
	if (t>=duration) {
		stop();
		current = endValue;
	} else {
		current = std::min(
			TweenPolicy::calc(startValue, endValue, c, t),
			endValue
		);
	}
	UpdatePolicy::update(current);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::setStartValue(const T &val) {
	startValue = val;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::setEndValue(const T &val) {
	endValue = val;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::setDuration(Millisecond d) {
	duration = d;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
void Animation<T,TW,UP>::setRefreshRate(Millisecond d) {
	Super::setDelay((TimeType)d);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class TW,
	template <class> class UP
>
typename Animation<T,TW,UP>::Ptr Animation<T,TW,UP>::create(const T &s, const T &e, 
	Millisecond d, Millisecond rfsh)
{
	Ptr res( new ThisClass() );
	res->setStartValue(s);
	res->setEndValue(e);
	res->setDuration(d);
	res->setRefreshRate(rfsh);
	res->EventSender<TimerEvent>::addEventListener(
		boost::bind(&ThisClass::update, res.get(), _1, _2)
	);
	res->self = res;
	return res;
}
}}} // namespace(s)

#endif /* SAMBAG_ANIMATON_H */
