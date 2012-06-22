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
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class X11WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
X11WindowToolkit::ToInvoke X11WindowToolkit::toInvoke;
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
	mainLoop();
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
///////////////////////////////////////////////////////////////////////////////
namespace { // thread / timer stuff
	//-------------------------------------------------------------------------
	boost::asio::io_service io;
	//-------------------------------------------------------------------------
	bool threadsAreRunning = true;
	//-------------------------------------------------------------------------
	void timerThreadClbk() {
		while (threadsAreRunning) {
			io.run();
			io.reset();
			boost::this_thread::sleep(boost::posix_time::millisec(10));
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
void X11WindowToolkit::
invokeLater(sambag::com::ICommand::Ptr cmd, long ms, int repetitions)
{
	Timer *t = new Timer(io, boost::posix_time::millisec(ms));
	toInvoke.insert( std::make_pair(t, cmd) );
	t->async_wait(
		boost::bind(&timerCallback,
		boost::asio::placeholders::error,
		t,
		ms,
		repetitions)
	);
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::timerCallback(const boost::system::error_code&,
		Timer *timer, long ms, int repetitions)
{
	ToInvoke::iterator it = toInvoke.find(timer);
	SAMBAG_ASSERT(it!=toInvoke.end());
	it->second->execute();
	if (repetitions == 1 || !threadsAreRunning) {
		toInvoke.erase(it);
		delete timer;
		return;
	}
	if (repetitions > 1)
		--repetitions;
	timer->expires_at(timer->expires_at() + boost::posix_time::millisec(ms));
	timer->async_wait(
		boost::bind(&timerCallback,
		boost::asio::placeholders::error,
		timer,
		ms,
		repetitions)
	);
}
//-------------------------------------------------------------------------
void X11WindowToolkit::closeAllTimer() {
	BOOST_FOREACH(ToInvoke::value_type &v, toInvoke) {
		v.first->cancel();
	}
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::mainLoop() {
	::Display *display = getToolkit()->getGlobals().display;
	threadsAreRunning = true;
	boost::thread timerThread(&timerThreadClbk);
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
	threadsAreRunning = false;
	closeAllTimer();
	timerThread.join();
	XCloseDisplay(display);
	display = NULL;

}
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return X11WindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_X11
