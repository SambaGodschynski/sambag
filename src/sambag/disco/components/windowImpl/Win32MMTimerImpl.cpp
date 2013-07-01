/*
 * Win32MMTimerImpl.cpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifdef _WIN32

#include "Win32MMTimerImpl.hpp"
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <windows.h>
#include <sambag/com/Common.hpp>
#include <Mmsystem.h>
#include <sambag/com/Exception.hpp>
namespace sambag { namespace disco {  namespace components {
namespace {
///////////////////////////////////////////////////////////////////////////////
typedef boost::bimap< boost::bimaps::unordered_set_of<MMRESULT>,
boost::bimaps::unordered_set_of<Timer::Ptr> > Timers;
Timers timers;
UINT period = 0;
//-----------------------------------------------------------------------------
/**
 * Windows timer callback.
 */
VOID CALLBACK timerProc(UINT idEvent, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{
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
		timeKillEvent(idEvent);
		timers.left.erase(it);
		return;
	}
	if (tm->getDelay() != tm->getInitialDelay()) { // initial delay
		// reset timer with new delay
		timeSetEvent(tm->getDelay(), period, 
		&timerProc, NULL, TIME_ONESHOT);
	}
}
} // namespace
//=============================================================================
//  Class Win32MMTimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
void Win32MMTimerImpl::closeAllTimer() {
	Timers::right_map::iterator it = timers.right.begin();
	while(it != timers.right.end()) {
		Timer::Ptr tm = it->first;
		MMRESULT idEvent = it->second;
		timeKillEvent(idEvent);
		// remove timermap entry and incr. it
		timers.right.erase(it);
		it = timers.right.begin(); 
	}
}
//-----------------------------------------------------------------------------
void Win32MMTimerImpl::startUpTimer() {
	TIMECAPS ptc = {0};
	MMRESULT res = timeGetDevCaps(&ptc, sizeof (TIMECAPS));
	SAMBAG_ASSERT(res==TIMERR_NOERROR);
	period = ptc.wPeriodMin;
	res = timeBeginPeriod(period);
	SAMBAG_ASSERT(res==TIMERR_NOERROR);
}
///----------------------------------------------------------------------------
void Win32MMTimerImpl::tearDownTimer() {
	closeAllTimer();
	MMRESULT res = timeEndPeriod(period);
	SAMBAG_ASSERT(res==TIMERR_NOERROR);
	period = 0;
}
//-----------------------------------------------------------------------------
void Win32MMTimerImpl::startTimer(Timer::Ptr tm) {
	MMRESULT timerId = timeSetEvent(tm->getInitialDelay(), period, 
		&timerProc, NULL, TIME_PERIODIC);
	SAMBAG_ASSERT(timerId);

	timers.insert(
		Timers::value_type(timerId, tm)
	);
}
//-----------------------------------------------------------------------------
void Win32MMTimerImpl::stopTimer(Timer::Ptr tm) {
	Timers::right_map::iterator it = timers.right.find(tm);
	if (it == timers.right.end())
		return;
	MMRESULT idEvent = it->second;
	MMRESULT res = timeKillEvent(idEvent);
	SAMBAG_ASSERT(res==TIMERR_NOERROR);
	timers.right.erase(it);
}
}}} // namespace(s)

#endif // _WIN32
