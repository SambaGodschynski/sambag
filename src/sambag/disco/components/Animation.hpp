/*
 * Animation.hpp
 *
 *  Created on: Wed Feb 13 14:14:30 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ANIMATON_H
#define SAMBAG_ANIMATON_H

#include <boost/shared_ptr.hpp>
#include <sambag/com/BoostTimer2.hpp>
#include <sambag/com/Common.hpp>
#include <sambag/disco/GenericAnimator.hpp>
#include <sambag/com/events/Events.hpp>

namespace sambag { namespace disco { namespace components {
struct DefaultTimerImpl : public sambag::com::BoostTimer2 {
    typedef sambag::com::BoostTimer2 Impl;
    void addListener(const sambag::com::events::EventSender<Impl::Event>::EventFunction &f)
    {
        sambag::com::events::EventSender<Impl::Event>::addEventListener(f);
    }
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
    typedef typename DefaultTimerImpl::Milliseconds Ms;
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
