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

namespace sambag { namespace disco { namespace components {
namespace ev = sambag::com::events;
class Timer;
//=============================================================================
struct TimerEvent {
	typedef boost::shared_ptr<Timer> TimerPtr;
	TimerPtr src;
	TimerEvent(TimerPtr src) : src(src) {}
	TimerPtr getSource() const { return src; }

};
//=============================================================================
/** 
  * @class Timer.
  */
class Timer : public ev::EventSender<TimerEvent> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef TimerEvent Event;
	//-------------------------------------------------------------------------
	typedef long TimeType; // ms
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Timer> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Timer> WPtr;
protected:
	//-------------------------------------------------------------------------
	Timer();
	//-------------------------------------------------------------------------
	int repetitions;
	//-------------------------------------------------------------------------
	TimeType delay, initialDelay;
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	bool running;
public:
	//-------------------------------------------------------------------------
	/**
	 * to be called by concrete WindowToolkit only.
	 * @param r
	 */
	void __setRunningByToolkit_(bool r) {
		running = r;
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
	static Ptr create(const TimeType &ms);
	//-------------------------------------------------------------------------
	/**
	 * @param delay in ms
	 */
	virtual void setDelay(const TimeType &delay);
	//-------------------------------------------------------------------------
	/**
	 * @param delay in ms
	 */
	virtual void setInitialDelay(const TimeType &delay);
	//-------------------------------------------------------------------------
	virtual const TimeType & getDelay() const {
		return delay;
	}
	//-------------------------------------------------------------------------
	virtual const TimeType & getInitialDelay() const {
		return initialDelay;
	}
	//-------------------------------------------------------------------------
	virtual int getNumRepetitions() const {
		return repetitions;
	}
	//-------------------------------------------------------------------------
	virtual void start();
	//-------------------------------------------------------------------------
	virtual void stop();
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param numRepeats number of repeates where -1 means infinite
	 */
	virtual void setNumRepetitions(int numRepeats);
	//-------------------------------------------------------------------------
	virtual bool isRunning() const;
}; // Timer
}}} // namespace(s)

#endif /* SAMBAG_TIMER_H */
