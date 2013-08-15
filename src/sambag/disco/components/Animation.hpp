/*
 * Animation.hpp
 *
 *  Created on: Wed Feb 13 14:14:30 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ANIMATON_H
#define SAMBAG_ANIMATON_H

#include <boost/shared_ptr.hpp>
#include <sambag/com/Common.hpp>
#include <sambag/disco/GenericAnimator.hpp>
#include <sambag/com/events/Events.hpp>
#include "Timer.hpp"

namespace sambag { namespace disco { namespace components {
namespace animationHelper {
    /**
     * extends TimerImpl with a addEventListener method.
     */
    template <class _TimerImpl>
    struct AddListenerForTimer : public _TimerImpl {
        typedef _TimerImpl TimerImpl;
        typedef typename TimerImpl::Event Event;
        typedef sambag::com::events::EventSender<Event> Sender;
        void addListener(const typename Sender::EventFunction &f)
        {
            Sender::addEventListener(f);
        }
    };
} // namespace(s)
////////////////////////////////////////////////////////////////////////////////
template < class T,
	template <class> class _Tween,
	template <class> class _UpdatePolicy,
    class _TimerImpl = sambag::disco::components::Timer
>
struct Animation :
    public GenericAnimator<T,
            animationHelper::AddListenerForTimer<_TimerImpl>,
            _Tween,
            _UpdatePolicy>
{
    typedef Animation<T, _Tween, _UpdatePolicy, _TimerImpl> ThisClass;
    typedef boost::shared_ptr<ThisClass> Ptr;
	typedef boost::weak_ptr<ThisClass> WPtr;
    typedef typename _TimerImpl::Milliseconds Ms;
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
