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

namespace sambag { namespace disco { namespace components {
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
	TimerPolicy::startThreads();
	while( GetMessage( &msg, NULL, 0, 0 ) > 0) 
	{
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	TimerPolicy::joinThreads();
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::startTimer(Timer::Ptr tm) {
	TimerPolicy::startTimer(tm);
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::stopTimer(Timer::Ptr tm) {
	TimerPolicy::stopTimer(tm);
}
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return Win32WindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_WIN32
