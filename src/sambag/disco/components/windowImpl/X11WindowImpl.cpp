/*
 * X11Window.cpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11WindowImpl.hpp"
#include <sambag/disco/X11Surface.hpp>
#include <boost/foreach.hpp>
#include "X11WindowToolkit.hpp"
#include <sambag/disco/components/WindowToolkit.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class X11WindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
X11WindowImpl::WinMap X11WindowImpl::winmap;
//-----------------------------------------------------------------------------
int X11WindowImpl::instances = 0;
//-----------------------------------------------------------------------------
Atom X11WindowImpl::wm_protocols_atom = 0;
//-----------------------------------------------------------------------------
Atom X11WindowImpl::wm_delete_window_atom = 0;
//-----------------------------------------------------------------------------
X11WindowImpl * X11WindowImpl::getX11WindowImpl(::Window win) {
	WinMap::iterator it = winmap.find(win);
	if (it==winmap.end())
		return NULL;
	return it->second;
}
//-----------------------------------------------------------------------------
void X11WindowImpl::initAsNestedWindow(ArbitraryType::Ptr osParent,
	const Rectangle &area) 
{
}
//-----------------------------------------------------------------------------
X11WindowImpl::X11WindowImpl() :
	bounds(Rectangle(0,0,1,1)),
	visible(false),
	screen(0),
	display(NULL),
	visual(NULL),
	win(0)
{
	display = X11WindowToolkit::getToolkit()->getGlobals().display;
	SAMBAG_ASSERT(display);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::createWindow() {
	if (instances++==0) {
		wm_protocols_atom = XInternAtom(display, "WM_PROTOCOLS", False);
		wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
		//XSynchronize(display, True);
	}
	screen = DefaultScreen(display);
	// init visual
	visual = XDefaultVisual(display, screen);

	SAMBAG_ASSERT(display);
	// create the window
	XSetWindowAttributes attributes;
	attributes.background_pixel = BlackPixel(display, screen);
	attributes.event_mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask
			| KeyReleaseMask
			| ButtonMotionMask
			| PointerMotionHintMask
			| StructureNotifyMask
			| PointerMotionMask
			| PointerMotionHintMask
	//		| ExposureMask
	;
	unsigned long valuemask = CWBackPixel | CWEventMask;
	if (!getFlag(WND_FRAMED)) {
		attributes.override_redirect = 1;
		valuemask |= CWOverrideRedirect;
	}
	win = XCreateWindow(display,
			RootWindow(display, screen),
			(int)bounds.x0().x(),
			(int)bounds.x0().y(),
			(unsigned int)bounds.getWidth(),
			(unsigned int)bounds.getHeight(),
			CopyFromParent,
			CopyFromParent, InputOutput, visual,
			valuemask, &attributes);
	// register win
	winmap[win] = this;
	// participate in the window manager 'delete yourself' protocol
	if (XSetWMProtocols(display, win, &wm_delete_window_atom, 1)==0) {
		SAMBAG_ASSERT(false);
	}
	// pop up the window
	XMapWindow(display, win);
	// force position
	XMoveWindow(display, win, (int)bounds.x0().x(),
			(int)bounds.x0().y());
	//XSync(display, win);
	visible = true;
	updateTitle();
	onCreated();
}
//-----------------------------------------------------------------------------
void X11WindowImpl::destroyWindow() {
	if (win==0)
		return;
	Ptr hold = self.lock();
	SAMBAG_ASSERT(hold);
	// unregister window
	winmap.erase(win);
	// X11's destroy
	onDestroy();
	XDestroyWindow(display, win);
	win = 0;
	--instances;
	visible = false;
}
//-----------------------------------------------------------------------------
void X11WindowImpl::close() {
	if (!visible)
		return;
	getWindowToolkit()->invokeLater(
		boost::bind(&X11WindowImpl::destroyWindow, this)
	);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::open(AWindowImplPtr parent) {
	if (visible)
		return;
	getWindowToolkit()->invokeLater(
		boost::bind(&X11WindowImpl::createWindow, this)
	);
}
//-----------------------------------------------------------------------------
X11WindowImpl::~X11WindowImpl() {
	//destroyWindow(); // TODO: formally close() unchecked behaviour
}
//-----------------------------------------------------------------------------
bool X11WindowImpl::isVisible() const {
	return visible;
}
//-----------------------------------------------------------------------------
Rectangle X11WindowImpl::getBounds() const {
	return bounds;
}
//-----------------------------------------------------------------------------
sambag::disco::ISurface::Ptr X11WindowImpl::createSurface() {
	using namespace sambag;
	Rectangle r = getBounds();
	return disco::X11Surface::create(display, win, visual,
			r.getWidth(), r.getHeight());
}
//-----------------------------------------------------------------------------
void X11WindowImpl::setBounds(const Rectangle &r) {
	Rectangle curr = getBounds();
	if (curr == r)
		return;
	bounds = r;

	if (bounds.getWidth() < 1.)
		bounds.setWidth(1.);
	if (bounds.getHeight() < 1.)
		bounds.setHeight(1.);

	if (!isVisible())
		return;

	updateBoundsToWindow();
}
//-----------------------------------------------------------------------------
void X11WindowImpl::invalidateWindow(const Rectangle &area) {
}
//-----------------------------------------------------------------------------
void X11WindowImpl::drawAll() {
	BOOST_FOREACH(WinMap::value_type &v, winmap) {
		sambag::disco::ISurface::Ptr s = v.second->createSurface();
		v.second->processDraw(s);
	}
}
//-----------------------------------------------------------------------------
void X11WindowImpl::setTitle(const std::string &_title) {
	title = _title;
	if (!isVisible())
		return;
	updateTitle();
}
//-----------------------------------------------------------------------------
std::string X11WindowImpl::getTitle() const {
	return title;
}
//-----------------------------------------------------------------------------
void X11WindowImpl::updateBoundsToWindow() {
	typedef unsigned int UI;
	XMoveResizeWindow(display, win, (int)bounds.x0().x(),
			(int)bounds.x0().y(),
			(UI)bounds.getWidth(),
			(UI)bounds.getHeight()
	);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::updateWindowToBounds(const Rectangle &r) {
	if (r.getDimension() != bounds.getDimension()) {
	}
	bounds = r;
	boundsUpdated();
}
//-----------------------------------------------------------------------------
void X11WindowImpl::updateTitle() {
	XTextProperty window_name;
	window_name.value = (unsigned char*) title.c_str();
	window_name.encoding = XA_STRING;
	window_name.format = 8;
	window_name.nitems = strlen((char *) window_name.value);
	XSetWMName(display, win, &window_name);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::handleEvent(XEvent &event) {
	::Display *display = X11WindowToolkit::getToolkit()->getGlobals().display;
	static int mx = 0, my = 0; // mouse position
	::Window win = event.xany.window;
	X11WindowImpl *src = getX11WindowImpl(win);
	if (!src)
		return;
	switch (event.type) {
	case ButtonPress: {
		int buttons = 0;
		if (event.xbutton.button == Button1)
			buttons |= 1;
		if (event.xbutton.button == Button2)
			buttons |= 4;
		if (event.xbutton.button == Button3)
			buttons |= 2;
		mx = event.xbutton.x;
		my = event.xbutton.y;
		src->handleMouseButtonPressEvent(mx, my, buttons);
	}
		return;

	case ButtonRelease: {
		int buttons = 0;
		if (event.xbutton.button == Button1)
			buttons |= 1;
		if (event.xbutton.button == Button2)
			buttons |= 4;
		if (event.xbutton.button == Button3)
			buttons |= 2;
		mx = event.xbutton.x;
		my = event.xbutton.y;
		src->handleMouseButtonReleaseEvent(mx, my, buttons);
	}
		return;

	case MotionNotify: {
		if (event.xmotion.is_hint) {
			::Window root, child;
			unsigned int mask;
			XQueryPointer(display, win, &root, &child, &event.xbutton.x_root,
					&event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
					&mask);
		}
		mx = event.xmotion.x;
		my = event.xmotion.y;
		src->handleMouseMotionEvent(mx, my);
	}
		return;


	case ClientMessage:
		if (event.xclient.message_type == wm_protocols_atom
				&& event.xclient.format == 32 && Atom(event.xclient.data.l[0])
				== wm_delete_window_atom) {
			src->close();
			return;
		}
		return;

	case ConfigureNotify: {
		Rectangle neu(event.xconfigure.x,
				event.xconfigure.y,
				event.xconfigure.width,
				event.xconfigure.height);
		src->updateWindowToBounds(neu);
		return;
	}
	case Expose:{
		//drawAll();
	}
	}
}
}}} // namespace(s)

#endif // DISCO_USE_X11
