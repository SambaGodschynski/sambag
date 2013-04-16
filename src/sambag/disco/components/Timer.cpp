/*
 * Timer.cpp
 *
 *  Created on: Mon Jun 25 10:35:04 2012
 *      Author: Johannes Unger
 */

#include "Timer.hpp"
#include "WindowToolkit.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Timer
//=============================================================================
//-----------------------------------------------------------------------------
Timer::Timer() :
	repetitions(0),
	delay(1),
	initialDelay(-1),
	running(false),
	numCalled(0)
{
}
//-----------------------------------------------------------------------------
Timer::~Timer() {
}
//-----------------------------------------------------------------------------
void Timer::__setRunningByToolkit_(bool r) {
	running = r;
	if (!running) {
		numCalled = 0;
	}
}
//-----------------------------------------------------------------------------
Timer::Ptr Timer::create(const TimeType &ms) {
	Ptr res(new Timer());
	res->self = res;
	res->setDelay(ms);
	return res;
}
//-----------------------------------------------------------------------------
void Timer::start() {
	if (initialDelay==-1)
		initialDelay = delay;
	getWindowToolkit()->startTimer(getPtr());
}
//-----------------------------------------------------------------------------
void Timer::stop() {
	getWindowToolkit()->stopTimer(getPtr());
}
//-----------------------------------------------------------------------------
void Timer::setNumRepetitions(int numRepeats) {
	repetitions = numRepeats;
}
//-----------------------------------------------------------------------------
bool Timer::isRunning() const {
	return running;
}
//-----------------------------------------------------------------------------
void Timer::setDelay(const TimeType &_delay) {
	delay = _delay;
}
//-----------------------------------------------------------------------------
void Timer::setInitialDelay(const TimeType &_delay) {
	initialDelay = _delay;
}
//-----------------------------------------------------------------------------
void Timer::timerExpired() {
	com::events::EventSender<TimerEvent>::notifyListeners(
			this,
			TimerEvent(getPtr())
	);
}
}}} // namespace(s)
