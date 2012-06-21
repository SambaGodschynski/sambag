/*
 * X11WindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11WindowToolkit.hpp"
#include "X11Window.hpp"
#include "WindowImpl.hpp"

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
		XCloseDisplay(globals.display);
		globals.display = NULL;
	}
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::startMainLoop() {
	X11WindowImpl::startMainLoop();
}
//-----------------------------------------------------------------------------
AWindowPtr X11WindowToolkit::createWindowImpl() const {
	AWindowPtr res = WindowImpl<X11WindowImpl>::create();
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
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return X11WindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_X11
