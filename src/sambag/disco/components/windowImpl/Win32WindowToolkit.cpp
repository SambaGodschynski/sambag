/*
 * Win32WindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_WIN32

#include "Win32WindowToolkit.hpp"
#include "WindowImpl.hpp"
#include "Win32WindowImpl.hpp"
#include <windows.h>


namespace sambag { namespace disco { namespace components {
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
	std::cout<<"11"<<std::endl;
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
//  Class Win32WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
Win32WindowToolkit::Win32WindowToolkit() {
}
//-----------------------------------------------------------------------------
Win32WindowToolkit::~Win32WindowToolkit() {
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::startMainLoop() {
	mainLoop();
}
//-----------------------------------------------------------------------------
AWindowImplPtr Win32WindowToolkit::createWindowImpl(AWindowImplPtr parent) const {
	AWindowImplPtr res = WindowImpl<Win32WindowImpl>::create(parent);
	return res;
}
//-----------------------------------------------------------------------------
Dimension Win32WindowToolkit::getScreenSize() const {
	return Dimension(GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
}
//-----------------------------------------------------------------------------
Win32WindowToolkit * Win32WindowToolkit::getToolkit() {
	typedef Loki::SingletonHolder<Win32WindowToolkit> FactoryHolder;
	return &FactoryHolder::Instance();
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::mainLoop() {
	MSG msg          = {0};
	while( GetMessage( &msg, NULL, 0, 0 ) > 0) 
	{
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::startTimer(Timer::Ptr tm) {
	tm->__setRunningByToolkit_(true);
	/*timers.insert(
		std::make_pair((UINT_PTR)tm.get(), TimerInfo(tm))
	);*/
	UINT_PTR p = SetTimer(NULL, NULL, 1, timerProc);
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::stopTimer(Timer::Ptr tm) {
	UINT_PTR idEvent = (UINT_PTR)tm.get();
	tm->__setRunningByToolkit_(false);
	Timers::iterator it = timers.find(idEvent);
	if (it!=timers.end()) {
		return;
	}
	KillTimer(NULL, idEvent);
}
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return Win32WindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_WIN32
