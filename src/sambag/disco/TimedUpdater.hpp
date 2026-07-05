/*
 * TimedUpdater.hpp
 *
 *  Created on: Sat Jan  5 13:45:21 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_PARAMETEROBSERVER_H
#define SAMBAG_PARAMETEROBSERVER_H

#include <loki/Singleton.h>
#include <sambag/disco/components/Timer.hpp>
#include <set>
#include <tuple>
#include <memory>
#include <sambag/com/Thread.hpp>
#include <thread>
#include <chrono>
#include <sambag/disco/components/WindowToolkit.hpp>

namespace sambag { namespace disco {

// Generic comparator for set keys that may contain std::weak_ptr elements.
// std::owner_less<T> only works for direct weak_ptr/shared_ptr — not for
// pair/tuple wrappers. This comparator handles those recursively.
namespace detail {
    template<class T> struct wptr_less {
        bool operator()(const T& a, const T& b) const { return std::less<T>{}(a, b); }
    };
    template<class T> struct wptr_less<std::weak_ptr<T>> {
        bool operator()(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b) const {
            return a.owner_before(b);
        }
    };
    template<class T, class U> struct wptr_less<std::pair<T,U>> {
        bool operator()(const std::pair<T,U>& a, const std::pair<T,U>& b) const {
            if (wptr_less<T>{}(a.first,  b.first))  return true;
            if (wptr_less<T>{}(b.first,  a.first))  return false;
            return wptr_less<U>{}(a.second, b.second);
        }
    };
    template<class... Ts> struct wptr_less<std::tuple<Ts...>> {
        template<std::size_t I, class Tup>
        static bool cmp(const Tup& a, const Tup& b) {
            if constexpr (I == sizeof...(Ts)) return false;
            else {
                using E = std::tuple_element_t<I, Tup>;
                if (wptr_less<E>{}(std::get<I>(a), std::get<I>(b))) return true;
                if (wptr_less<E>{}(std::get<I>(b), std::get<I>(a))) return false;
                return cmp<I+1>(a, b);
            }
        }
        bool operator()(const std::tuple<Ts...>& a, const std::tuple<Ts...>& b) const {
            return cmp<0>(a, b);
        }
    };
} // namespace detail

struct UpdaterUsingTimer {
	typedef sambag::disco::components::Timer RefreshTimer;
    RefreshTimer::Ptr refreshTimer;
    template <class Function>
    void init(int refreshTime, const Function &f) {
        using namespace sambag::disco::components;
        namespace sce = sambag::com::events;
        refreshTimer = RefreshTimer::create(refreshTime);
        refreshTimer->sce::EventSender<RefreshTimer::Event>::addEventListener(f);
        refreshTimer->setNumRepetitions(-1);
        refreshTimer->start();
    }
    bool isReady() {
        return refreshTimer.get() != NULL;
    }
};
struct UpdaterUsingThread {
    template <class Function>
    struct Thread {
        typedef Function F;
        typedef Thread<F> Class;
        Function f;
        int refreshTime;
        bool running;
        std::thread *thread;
        Thread(int refreshTime, const Function &f) :
            f(f),
            refreshTime(refreshTime)
        {
            running = true;
            thread = new std::thread(&Class::onUpdate, this);
        }
        ~Thread() {
            running = false;
            thread->join();
            delete thread;
        }
        void onUpdate() {
            while(running) {
                f();
                std::this_thread::sleep_for(std::chrono::milliseconds(refreshTime));
            }
        }
    };
    std::shared_ptr<void> thread;
    template <class Function>
    void init(int refreshTime, const Function &f) {
        thread = std::shared_ptr<void>(
            new Thread<Function>(refreshTime, f)
        );
    }
    bool isReady() {
        return thread.get() != NULL;
    }
};
//=============================================================================
/** 
  * @class TimedUpdater.
  * updates chaged value at appropriate time.
  *
  * DoUpdatePolicy concept:
  * 
  * DoUpdatePolicy {
  *     // if returning false Value is concerned as not updated
  *		bool update( const Value &v );
  * };
  */
template <
	class T,
	template <class> class _DoUpdatePolicy,
	int _RefreshTime,
    class _UpdaterPolicy = UpdaterUsingTimer
>
class TimedUpdater : public _DoUpdatePolicy<T>, public _UpdaterPolicy {
//=============================================================================
friend struct Loki::CreateUsingNew<TimedUpdater>;
public:
    //-------------------------------------------------------------------------
    typedef _UpdaterPolicy UpdaterPolicy;
	//-------------------------------------------------------------------------
	enum { RefreshTime = _RefreshTime };
	//-------------------------------------------------------------------------
	typedef T UpdateValueType;
	//-------------------------------------------------------------------------
	typedef _DoUpdatePolicy<UpdateValueType> DoUpdatePolicy;
	//-------------------------------------------------------------------------
	typedef std::set<UpdateValueType, detail::wptr_less<UpdateValueType>> Values;
	//-------------------------------------------------------------------------
	typedef TimedUpdater<UpdateValueType, _DoUpdatePolicy, RefreshTime, UpdaterPolicy> Class;
protected:
	//-------------------------------------------------------------------------
	void doUpdate();
private:
	//-------------------------------------------------------------------------
	sambag::com::Mutex mutex;
	//-------------------------------------------------------------------------
	Values values;
	//-------------------------------------------------------------------------
	TimedUpdater(){}
public:
	//-------------------------------------------------------------------------
	void update(const UpdateValueType &data) {
		{
			SAMBAG_TRY_TO_LOCK_TIMED(mutex)
			values.insert(data);
		}
		if (!UpdaterPolicy::isReady()) {
			UpdaterPolicy::init(RefreshTime, [this](void*, const sambag::disco::components::TimerEvent&){ this->doUpdate(); });
		}
	}
	//-------------------------------------------------------------------------
	static TimedUpdater & instance() {
		typedef Loki::SingletonHolder<TimedUpdater> Holder;
		return Holder::Instance();
	}
}; // TimedUpdater
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <
	class V, 
	template <class> class P,
	int T,
    class U
>
void TimedUpdater<V,P,T,U>::doUpdate()
{
	while (!values.empty()) {
        SAMBAG_TRY_TO_LOCK_TIMED(mutex)
        typename Values::iterator it = values.begin();
		if (DoUpdatePolicy::update(*it)) {
            values.erase(*it);
        }
	}
}
}} // namespace(s)

#endif /* SAMBAG_PARAMETEROBSERVER_H */
