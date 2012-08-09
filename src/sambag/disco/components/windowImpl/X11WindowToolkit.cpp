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
	TimerPolicy::startThreads();
	while ( X11WindowImpl::getNumInstances() > 0 ) {
		// read in and process all pending events for the main window
		XEvent event;
		while (X11WindowImpl::getNumInstances() > 0 && XPending(display)) {
			XNextEvent(display, &event);
			X11WindowImpl::handleEvent(event);
		}
		X11WindowImpl::drawAll();
		usleep(1000);
		X11WindowImpl::processInvocations();
	}
	TimerPolicy::joinThreads();
	XCloseDisplay(display);
	display = NULL;

}
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return X11WindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_X11
