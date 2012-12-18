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
#include <sambag/disco/components/Window.hpp>
#include <sambag/disco/components/Timer.hpp>
#include <windows.h>
#include <boost/thread.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Win32WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
bool Win32WindowToolkit::mainLoopRunning = false;
//-----------------------------------------------------------------------------
Win32WindowToolkit::Win32WindowToolkit() {
}
//-----------------------------------------------------------------------------
Win32WindowToolkit::~Win32WindowToolkit() {
}
namespace {
	void checkWindowCount() {
		// if no window registred, close program
		if (Window::getOpenWindows().size() == 0) {
			sambag::com::log("no window registred, exit mainloop");
			PostQuitMessage(0);
		}
	}
} // namespace(s)
//-----------------------------------------------------------------------------
void Win32WindowToolkit::startMainLoop() {
	invokeLater(&checkWindowCount, 1000);
	mainLoop();
}
//-----------------------------------------------------------------------------
AWindowImplPtr Win32WindowToolkit::createWindowImpl(AWindowImplPtr parent) const {
	AWindowImplPtr res = WindowImpl<Win32WindowImpl>::create(parent);
	return res;
}
//-------------------------------------------------------------------------
WindowPtr Win32WindowToolkit::createNestedWindow( ArbitraryType::Ptr osParent, 
	const Rectangle &area )
{
	AWindowImpl::Ptr windowImpl = 
		WindowImpl<Win32WindowImpl>::create(osParent, area);
	return Window::create(windowImpl);
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
void Win32WindowToolkit::useWithoutMainloop() {
	TimerPolicy::startUpTimer();
}
//-----------------------------------------------------------------------------
void Win32WindowToolkit::mainLoop() {
	MSG msg          = {0};
	mainLoopRunning = true;
	TimerPolicy::startUpTimer();
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	mainLoopRunning = false;
	TimerPolicy::tearDownTimer();
}
namespace {
	void quitImpl() {
		PostQuitMessage(0);
	}
} // namespace(s)
//-----------------------------------------------------------------------------
void Win32WindowToolkit::quit() {
	invokeLater(&quitImpl);
}
//-----------------------------------------------------------------------------
namespace {
	/* 
	 * timer callback
	 */
	void _invokeLater(Win32WindowToolkit::InvokeFunction f) 
	{
		f();
	}
} // namespace(s)
void Win32WindowToolkit::invokeLater(const Win32WindowToolkit::InvokeFunction &f) 
{
	invokeLater(f, 50);
}
//----------------------------------------------------------------------------
void Win32WindowToolkit::invokeLater(const InvokeFunction &f, int ms) {
	Timer::Ptr tm = Timer::create(ms);
	tm->setNumRepetitions(0);
	tm->EventSender<TimerEvent>::addEventListener(
		boost::bind(&_invokeLater, f)
	);
	tm->start();
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
