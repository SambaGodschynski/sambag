/*
 * X11WindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11WindowToolkit.hpp"
#include "X11WindowImpl.hpp"
#include "WindowImpl.hpp"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class X11WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
bool X11WindowToolkit::mainLoopRunning = false;
//-----------------------------------------------------------------------------
X11WindowToolkit::InvokeLater X11WindowToolkit::_invokeLater;
//-----------------------------------------------------------------------------
X11WindowToolkit::X11WindowToolkit() {
	globals.display = XOpenDisplay(NULL);
}
//-----------------------------------------------------------------------------
X11WindowToolkit::~X11WindowToolkit() {
	if (globals.display) {
		// TODO: XCloseDisplay causes segmentation fault
		//XCloseDisplay(globals.display);
		globals.display = NULL;
	}
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::invokeWaiting() {
	while (!_invokeLater.empty()) {
		const InvokeFunction &f = _invokeLater.front();
		f();
		_invokeLater.pop();
	}
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::startMainLoop() {
	mainLoop();
}
//-----------------------------------------------------------------------------
AWindowImplPtr X11WindowToolkit::createWindowImpl(AWindowImplPtr parent) const {
	AWindowImplPtr res = WindowImpl<X11WindowImpl>::create(parent);
	return res;
}
//-------------------------------------------------------------------------
const X11WindowToolkit::Globals & X11WindowToolkit::getGlobals() const {
	return globals;
}
//-----------------------------------------------------------------------------
Dimension X11WindowToolkit::getScreenSize() const {
	::Display * dsp = globals.display;
	SAMBAG_ASSERT(dsp);
	::Screen *scr = XScreenOfDisplay(dsp, XDefaultScreen(dsp));
	int w = XWidthOfScreen(scr);
	int h = XHeightOfScreen(scr);
	return Dimension((Coordinate)w, (Coordinate)h);
}
//-----------------------------------------------------------------------------
X11WindowToolkit * X11WindowToolkit::getToolkit() {
	typedef Loki::SingletonHolder<X11WindowToolkit> X11WindowFactoryHolder;
	return &X11WindowFactoryHolder::Instance();
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::startTimer(Timer::Ptr tm) {
	TimerPolicy::startTimer(tm);
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::stopTimer(Timer::Ptr tm) {
	TimerPolicy::stopTimer(tm);
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::mainLoop() {
	::Display *display = getToolkit()->getGlobals().display;
	TimerPolicy::startUpTimer();
	mainLoopRunning = true;
	while ( (X11WindowImpl::getNumInstances() > 0) && mainLoopRunning ) {
		// read in and process all pending events for the main window
		XEvent event;
		while (X11WindowImpl::getNumInstances() > 0 && XPending(display)) {
			XNextEvent(display, &event);
			X11WindowImpl::handleEvent(event);
		}
		X11WindowImpl::drawAll();
		usleep(1000);
		invokeWaiting();
	}
	mainLoopRunning = false;
	TimerPolicy::tearDownTimer();
	XCloseDisplay(display);
	display = NULL;

}
//-----------------------------------------------------------------------------
void X11WindowToolkit::quit() {
	mainLoopRunning = false;
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::invokeLater(const X11WindowToolkit::InvokeFunction &f, int ms,
    boost::shared_ptr<void> toTrack)
{
	if (!isMainLoopRunning()) {
		f();
		return;
	}
	_invokeLater.push(f);
}
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return X11WindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_X11
