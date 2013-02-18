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
#include <sambag/com/exceptions/IllegalStateException.hpp>
#include <stack>
#include <boost/thread.hpp>

namespace sambag { namespace disco {  namespace components {

namespace {
///////////////////////////////////////////////////////////////////////////////
typedef boost::bimap< boost::bimaps::unordered_set_of<UINT_PTR>,
boost::bimaps::unordered_set_of<Timer::Ptr> > Timers;
Timers timers;
///////////////////////////////////////////////////////////////////////////////
/*
	the problem: you can't call SetTimer in another thread than the
	thread with the mainloop (messagepump). so we need a
	masterTimer which proc(in main thread) starts the requested timer.
*/
typedef boost::function<void()> Proc;
typedef std::stack<Proc> Procs;
boost::mutex procMutex;
Procs procs;
UINT_PTR masterTimerId = 0;
VOID CALLBACK masterTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) 
{
	while (!procs.empty()) {
		boost::lock_guard<boost::mutex> lock(procMutex);
		procs.top()(); // call proc
		procs.pop();
	}
}
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
	UINT_PTR timerId = 0;
	Timers::right_map::iterator it = timers.right.find(tm);
	if (it==timers.right.end()) {
		timerId = SetTimer(NULL, 0, tm->getInitialDelay(), timerProc);
	} else {
		timerId = SetTimer(NULL, it->second, tm->getInitialDelay(), timerProc);
		if (timerId!=NULL) {
			return;
		}
	}
	if (timerId==NULL) {
		std::stringstream ss;
		ss<<"could'nt create timer (GetLastError="<<GetLastError()<<")";
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			ss.str().c_str()
		);
	}
	timers.insert(
		Timers::value_type(timerId, tm)
	);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::stopTimerImpl(Timer::Ptr tm) {
	tm->__setRunningByToolkit_(false);
	Timers::right_map::iterator it = timers.right.find(tm);
	if (it == timers.right.end())
		return;
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
void Win32TimerImpl::stopTimer(Timer::Ptr tm) {
	boost::lock_guard<boost::mutex> lock(procMutex);
	procs.push(
		boost::bind(&Win32TimerImpl::stopTimerImpl, this, tm)
	);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::startUpTimer() {
	if (masterTimerId!=0) {
		return;
	}
	masterTimerId = SetTimer(NULL, NULL, 10, masterTimer);
	if (masterTimerId==NULL) {
		std::stringstream ss;
		ss<<"could'nt create timer (GetLastError="<<GetLastError()<<")";
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			ss.str().c_str()
		);
	}
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::tearDownTimer() {
	closeAllTimer();
	if (masterTimerId!=0) {
		KillTimer(NULL, masterTimerId);
		masterTimerId = 0;
	}

}
}}} // namespace(s)

#endif // _WIN32
