/*
 * Win32TimerImpl.cpp
 *
 *  Created on: Tue Aug  7 09:41:46 2012
 *      Author: Johannes Unger
 */

#ifdef _WIN32

#include "Win32TimerImpl.hpp"
#include <boost/unordered_map.hpp>
#include <windows.h>
#include <sambag/com/Common.hpp>

namespace sambag { namespace disco {  namespace components {
namespace {
///////////////////////////////////////////////////////////////////////////////
// Timer impl.
struct TimerInfo {
	Timer::Ptr tm;
	int numCalled;
	TimerInfo(Timer::Ptr tm) : tm(tm), numCalled(0) {}
};
typedef boost::unordered_map<UINT_PTR, TimerInfo> Timers;
Timers timers;
//-----------------------------------------------------------------------------
/**
 * Windows timer callback.
 */
VOID CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	std::cout<<"bug: 'never called' is fixed now"<<std::endl;
	Timers::iterator it = timers.find(idEvent);
	if (it!=timers.end()) {
		return;
	}
	Timer::Ptr tm = it->second.tm;
	tm->timerExpired();
	++(it->second.numCalled);
	if (tm->getNumRepetitions() != -1 &&
		it->second.numCalled > tm->getNumRepetitions()) 
	{
		tm->__setRunningByToolkit_(false);
		KillTimer(hwnd, idEvent);
		timers.erase(it);
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
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::startTimer(Timer::Ptr tm) {
	tm->__setRunningByToolkit_(true);
	/*
	 * the invoke below dosent't do anything.
	 * A timerId greater zero comes back, but there is
	 * no WM_TIMER call. ( even in the mainloop )
	 */
	UINT_PTR timerId = SetTimer(NULL, NULL, 1, timerProc);
	timers.insert(
		std::make_pair(timerId, TimerInfo(tm))
	);
}
//-----------------------------------------------------------------------------
void Win32TimerImpl::stopTimer(Timer::Ptr tm) {
	SAMBA_LOG_NOT_YET_IMPL();
}
}}} // namespace(s)

#endif // _WIN32
