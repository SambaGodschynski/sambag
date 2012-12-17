/*
 * Win32TimerImpl.cpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifdef _WIN32

#include "Win32TimerImpl.hpp"
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <windows.h>
#include <sambag/com/Common.hpp>

namespace sambag { namespace disco {  namespace components {

namespace {
///////////////////////////////////////////////////////////////////////////////
typedef boost::bimap< boost::bimaps::unordered_set_of<UINT_PTR>,
boost::bimaps::unordered_set_of<Timer::Ptr> > Timers;
Timers timers;
//-----------------------------------------------------------------------------
/**
 * Windows timer callback.
 */
VOID CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	Timers::left_map::iterator it = timers.left.find(idEvent);
	if (it==timers.left.end()) {
		return;
	}
	Timer::Ptr tm = it->second;
	tm->timerExpired();
	int &numCalled = tm->__getNumCalled_();
	++numCalled;
	if (tm->getNumRepetitions() != -1 &&
		numCalled > tm->getNumRepetitions()) 
	{
		tm->__setRunningByToolkit_(false);
		KillTimer(hwnd, idEvent);
		timers.left.erase(it);
		return;
	}
	if (tm->getDelay() != dwTime) { // initial delay
		// reset timer with new delay
		SetTimer(NULL, idEvent, tm->getDelay(), &timerProc);
	}
}
} // namespace
//=============================================================================
//  Class Win32TimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
boost::mutex Win32TimerImpl::procMutex;
//-----------------------------------------------------------------------------
Win32TimerImpl::Procs Win32TimerImpl::procs;
//-----------------------------------------------------------------------------
boost::thread::id Win32TimerImpl::mainLoopId;
//-----------------------------------------------------------------------------
void Win32TimerImpl::closeAllTimer() {
	Timers::right_map::iterator it = timers.right.begin();
	while(it != timers.right.end()) {
		Timer::Ptr tm = it->first;
		tm->__setRunningByToolkit_(false);
		UINT_PTR idEvent = it->second;
		KillTimer(NULL, idEvent);
		// remove timermap entry and incr. it
		timers.right.erase(it);
		it = timers.right.begin(); 
	}
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::startTimerImpl(Timer::Ptr tm) {
	tm->__setRunningByToolkit_(true);
	UINT_PTR timerId = SetTimer(NULL, NULL, tm->getInitialDelay(), timerProc);
	timers.insert(
		Timers::value_type(timerId, tm)
	);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::stopTimerImpl(Timer::Ptr tm) {
	Timers::right_map::iterator it = timers.right.find(tm);
	if (it == timers.right.end())
		return;
	tm->__setRunningByToolkit_(false);
	UINT_PTR idEvent = it->second;
	KillTimer(NULL, idEvent);
	timers.right.erase(it);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::startTimer(Timer::Ptr tm) {
	boost::lock_guard<boost::mutex> lock(procMutex);
	procs.push(
		boost::bind(&Win32TimerImpl::startTimerImpl, this, tm)
	);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::startUpTimer(boost::thread::id id) {
	mainLoopId = id;
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::stopTimer(Timer::Ptr tm) {
	boost::lock_guard<boost::mutex> lock(procMutex);
	procs.push(
		boost::bind(&Win32TimerImpl::stopTimerImpl, this, tm)
	);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::mainLoopProc() {
	boost::lock_guard<boost::mutex> lock(procMutex);
	while (!procs.empty()) {
		procs.top()(); // call proc
		procs.pop();
	}
}
}}} // namespace(s)

#endif // _WIN32
