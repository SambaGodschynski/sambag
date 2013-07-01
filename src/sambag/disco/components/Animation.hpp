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
#include <sambag/disco/GenericAnimator.hpp>

namespace sambag { namespace disco { namespace components {
class DefaultTimerImpl : public Timer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef Timer Super;
	//-------------------------------------------------------------------------
	typedef TimeType Millisecond;
	//-------------------------------------------------------------------------
	typedef DefaultTimerImpl ThisClass;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ThisClass> Ptr;
protected:
	//-------------------------------------------------------------------------
	DefaultTimerImpl() {}
private:
public:
	//-------------------------------------------------------------------------
	void setRefreshRate(Millisecond d);
	//-------------------------------------------------------------------------
	Millisecond getRefreshRate() const {
		return (Millisecond)Super::getDelay();
	}
    //-------------------------------------------------------------------------
    typedef Super::Event TimerEvent;
    //-------------------------------------------------------------------------
    typedef com::events::EventSender<Super::Event>::EventFunction EventFunction;
    //-------------------------------------------------------------------------
    void addTimerListener(const EventFunction &f);
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
}; // DefaultTimerImpl
////////////////////////////////////////////////////////////////////////////////
template < class T,
	template <class> class _Tween,
	template <class> class _UpdatePolicy
>
struct Animation :
    public GenericAnimator<T, DefaultTimerImpl, _Tween, _UpdatePolicy>
{
    typedef Animation<T, _Tween, _UpdatePolicy> ThisClass;
    typedef boost::shared_ptr<ThisClass> Ptr;
    typedef typename DefaultTimerImpl::Millisecond Ms;
    static Ptr create(const T &s=T(), const T &e=T(), Ms d=0, Ms rfsh=0)
    {
        Ptr res( new ThisClass() );
        res->setStartValue(s);
        res->setEndValue(e);
        res->setDuration(d);
        res->setRefreshRate(rfsh);
        res->self = res;
        return res;
    }

};
}}} // namespace(s)

#endif /* SAMBAG_ANIMATON_H */
