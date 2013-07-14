/*
 * ITimer.hpp
 *
 *  Created on: Sun Jul 14 12:26:17 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ITIMER_H
#define SAMBAG_ITIMER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace sambag { namespace com {
//=============================================================================
/** 
  * @class ITimer.
  */
class ITimer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ITimer> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<ITimer> WPtr;
    //-------------------------------------------------------------------------
    typedef long Milliseconds;
	//-------------------------------------------------------------------------
	/**
	 * to be called by concrete TimerImpl only.
	 */
	virtual int & __getNumCalled_() = 0;
	//-------------------------------------------------------------------------
	virtual int getNumCalled() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * to be called by concrete WindowToolkit only.
	 */
	virtual void timerExpired() = 0;
	//-------------------------------------------------------------------------
	/**
	 * @param delay in ms
	 */
	virtual void setDelay(const Milliseconds &delay) = 0;
	//-------------------------------------------------------------------------
	/**
	 * @param delay in ms
	 */
	virtual void setInitialDelay(const Milliseconds &delay) = 0;
	//-------------------------------------------------------------------------
	virtual const Milliseconds & getDelay() const = 0;
	//-------------------------------------------------------------------------
	virtual const Milliseconds & getInitialDelay() const = 0;
	//-------------------------------------------------------------------------
	virtual int getNumRepetitions() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * starts the timer.
	 * @note: if timer is already running the timer reset and restarts.
	 * (TODO: check this behaviour with BoostTimerImpl)
	 */
	virtual void start() = 0;
	//-------------------------------------------------------------------------
	/**
	 * stops the timer.
	 */
	virtual void stop() = 0;
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param numRepeats number of repeates where -1 means infinite
	 */
	virtual void setNumRepetitions(int numRepeats) = 0;
	//-------------------------------------------------------------------------
	virtual bool isRunning() const = 0;

}; // ITimer
}} // namespace(s)

#endif /* SAMBAG_ITIMER_H */
