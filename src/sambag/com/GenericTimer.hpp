/*
 * GenericTimer.hpp
 *
 *  Created on: Sun Jul 14 11:34:19 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GENERICTIMER_H
#define SAMBAG_GENERICTIMER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "events/Events.hpp"
#include "ITimer.hpp"

namespace sambag { namespace com {
//=============================================================================
/** 
  * @class GenericTimer.
  *
  * EventPolicy:
  *     template <class Timer> fireEvent(const Timer &t)
  *
  * TimerImplPolicy:
  *
  *    template <class Timer> startTimer(const Timer &t);
  *    template <class Timer> stopTimer(const Timer &t);   
  */
template <
    class _EventPolicy,
    class _TimerImplPolicy
>
class GenericTimer : public ITimer,
                     public _EventPolicy,
                     public _TimerImplPolicy
{
//=============================================================================
public:
    //-------------------------------------------------------------------------
    /**
     * @deprecated use Milliseconds;
     */
    typedef long TimeType;
    typedef long Milliseconds;
	//-------------------------------------------------------------------------
	typedef _EventPolicy EventPolicy;
    //-------------------------------------------------------------------------
    typedef _TimerImplPolicy TimerImplPolicy;
    //-------------------------------------------------------------------------
    typedef GenericTimer<EventPolicy, TimerImplPolicy> ThisClass;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ThisClass> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<ThisClass> WPtr;
protected:
	//-------------------------------------------------------------------------
	GenericTimer();
	//-------------------------------------------------------------------------
	int repetitions;
	//-------------------------------------------------------------------------
	TimeType delay, initialDelay;
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	bool running;
	//-------------------------------------------------------------------------
	int numCalled;
private:
public:
	//-------------------------------------------------------------------------
	virtual ~GenericTimer();
	//-------------------------------------------------------------------------
	/**
	 * to be called by concrete TimerImpl only.
	 */
	int & __getNumCalled_() {
		return numCalled;
	}
	//-------------------------------------------------------------------------
	int getNumCalled() const {
		return numCalled;
	}
	//-------------------------------------------------------------------------
	/**
	 * to be called by concrete WindowToolkit only.
	 */
	virtual void timerExpired();
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	static Ptr create(const Milliseconds &ms);
	//-------------------------------------------------------------------------
	/**
	 * @param delay in ms
	 */
	virtual void setDelay(const Milliseconds &delay);
	//-------------------------------------------------------------------------
	/**
	 * @param delay in ms
	 */
	virtual void setInitialDelay(const Milliseconds &delay);
	//-------------------------------------------------------------------------
	virtual const Milliseconds & getDelay() const {
		return delay;
	}
	//-------------------------------------------------------------------------
	virtual const Milliseconds & getInitialDelay() const {
		return initialDelay;
	}
	//-------------------------------------------------------------------------
	virtual int getNumRepetitions() const {
		return repetitions;
	}
	//-------------------------------------------------------------------------
	/**
	 * starts the timer.
	 * @note: if timer is already running the timer reset and restarts.
	 * (TODO: check this behaviour with BoostTimerImpl)
	 */
	virtual void start();
	//-------------------------------------------------------------------------
	/**
	 * stops the timer.
	 */
	virtual void stop();
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param numRepeats number of repeates where -1 means infinite
	 */
	virtual void setNumRepetitions(int numRepeats);
	//-------------------------------------------------------------------------
	virtual bool isRunning() const;
}; // GenericTimer
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
GenericTimer<Ev, Impl>::GenericTimer() :
	repetitions(0),
	delay(1),
	initialDelay(-1),
	running(false),
	numCalled(0)
{
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
GenericTimer<Ev, Impl>::~GenericTimer() {
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
typename GenericTimer<Ev, Impl>::Ptr
GenericTimer<Ev, Impl>::create(const Milliseconds &ms)
{
	Ptr res(new GenericTimer());
	res->self = res;
	res->setDelay(ms);
	return res;
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
void GenericTimer<Ev, Impl>::start() {
	if (initialDelay==-1)
		initialDelay = delay;
    running = true;
    numCalled = 0;
	TimerImplPolicy::startTimer(*this);
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
void GenericTimer<Ev, Impl>::stop() {
    running = false;
	TimerImplPolicy::stopTimer(*this);
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
void GenericTimer<Ev, Impl>::setNumRepetitions(int numRepeats) {
	repetitions = numRepeats;
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
bool GenericTimer<Ev, Impl>::isRunning() const {
	return running;
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
void GenericTimer<Ev, Impl>::setDelay(const Milliseconds &_delay) {
	delay = _delay;
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
void GenericTimer<Ev, Impl>::setInitialDelay(const Milliseconds &_delay) {
	initialDelay = _delay;
}
//-----------------------------------------------------------------------------
template <class Ev, class Impl>
void GenericTimer<Ev, Impl>::timerExpired() {
    if (!isRunning()) {
        return;
    }
	EventPolicy::fireEvent(*this);
}
}} // namespace(s)

#endif /* SAMBAG_GENERICTIMER_H */
