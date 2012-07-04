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
#include <sambag/com/Thread.hpp>
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
	sambag::com::RecursiveMutex timerLock;
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
void X11WindowToolkit::startTimer(Timer::Ptr tm) {
	Timer::TimeType ms = tm->getInitialDelay();
	int repetitions = tm->getNumRepetitions();
	TimerImpl *t = new TimerImpl(io, boost::posix_time::millisec(ms));
	toInvoke.insert( ToInvoke::value_type(t, tm) );
	t->async_wait(
		boost::bind(&timerCallback,
		boost::asio::placeholders::error,
		t,
		repetitions)
	);
	tm->__setRunningByToolkit_(true);
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::stopTimer(Timer::Ptr tm) {
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		ToInvoke::right_map::iterator it = toInvoke.right.find(tm);
		if (it==toInvoke.right.end()) // timerImpl not found
			return;
		TimerImpl *timerImpl = it->second;
		tm->__setRunningByToolkit_(false);
		timerImpl->cancel();
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void X11WindowToolkit::timerCallback(const boost::system::error_code&,
		TimerImpl *timerImpl, int repetitions)
{
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		ToInvoke::left_map::iterator it = toInvoke.left.find(timerImpl);
		if (it==toInvoke.left.end())
			return;
		Timer::Ptr tm = it->second;
		tm->timedExpired();
		if (repetitions == 0 ||
			!threadsAreRunning ||
			!tm->isRunning()) // stop forced
		{
			tm->__setRunningByToolkit_(false);
			toInvoke.left.erase(it);
			delete timerImpl;
			return;
		}
		if (repetitions > 0)
			--repetitions;
		long ms = tm->getDelay();
		timerImpl->expires_at(timerImpl->expires_at() + boost::posix_time::millisec(ms));
		timerImpl->async_wait(
			boost::bind(&timerCallback,
			boost::asio::placeholders::error,
			timerImpl,
			repetitions)
		);
	SAMBAG_END_SYNCHRONIZED
}
//-------------------------------------------------------------------------
void X11WindowToolkit::closeAllTimer() {
	SAMBAG_BEGIN_SYNCHRONIZED(timerLock)
		BOOST_FOREACH(ToInvoke::left_map::value_type &v, toInvoke.left) {
			v.first->cancel();
		}
	SAMBAG_END_SYNCHRONIZED
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
