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
#include <sambag/com/Thread.hpp>
#include <boost/function.hpp>
#include <sambag/com/Thread.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>

namespace sambag { namespace disco {
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
        boost::thread *thread;
        Thread(int refreshTime, const Function &f) :
            f(f),
            refreshTime(refreshTime)
        {
            running = true;
            thread = new boost::thread(boost::bind(&Class::onUpdate, this));
        }
        ~Thread() {
            running = false;
            thread->join();
            delete thread;
        }
        void onUpdate() {
            while(running) {
                f();
                boost::this_thread::sleep(boost::posix_time::milliseconds(refreshTime));
            }
        }
    };
    boost::shared_ptr<void> thread;
    template <class Function>
    void init(int refreshTime, const Function &f) {
        thread = boost::shared_ptr<void>(
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
	typedef std::set<UpdateValueType> Values;
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
			UpdaterPolicy::init(RefreshTime, boost::bind(&Class::doUpdate, this));
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
