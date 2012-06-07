/*
 * X11Window.cpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11Window.hpp"
#include "X11Surface.hpp"
#include <boost/foreach.hpp>

namespace {
	void microsleep(int usecs) {
		usleep(usecs);
	}
}

namespace sambag { namespace disco {
//=============================================================================
// struct DestroyWindow
//=============================================================================
void DestroyWindow::execute() {
	dst->destroyWindow();
}
//=============================================================================
//  Class X11WindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
X11WindowImpl::WinMap X11WindowImpl::winmap;
//-----------------------------------------------------------------------------
Display * X11WindowImpl::display = NULL;
//-----------------------------------------------------------------------------
int X11WindowImpl::instances = 0;
//-----------------------------------------------------------------------------
Atom X11WindowImpl::wm_protocols_atom = 0;
//-----------------------------------------------------------------------------
Atom X11WindowImpl::wm_delete_window_atom = 0;
//-----------------------------------------------------------------------------
X11WindowImpl::Invocations X11WindowImpl::invocations;
//-----------------------------------------------------------------------------
int X11WindowImpl::screen = 0;
//-----------------------------------------------------------------------------
Visual * X11WindowImpl::visual = NULL;
//-----------------------------------------------------------------------------
X11WindowImpl * X11WindowImpl::getX11WindowImpl(Window win) {
	WinMap::iterator it = winmap.find(win);
	if (it==winmap.end())
		return NULL;
	return it->second;
}
//-----------------------------------------------------------------------------
X11WindowImpl::X11WindowImpl() :
	visible(false),
	win(0)
{
}
//-----------------------------------------------------------------------------
void X11WindowImpl::createWindow(bool framed) {
	if (instances++==0) {
		display = XOpenDisplay(NULL);
		SAMBAG_ASSERT(display);
		wm_protocols_atom = XInternAtom(display, "WM_PROTOCOLS", False);
		wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
		screen = DefaultScreen(display);
		// init visual
		visual = XDefaultVisual(display, 0);
	}
	SAMBAG_ASSERT(display);

	// initialize variables
	win = 0;

	// create the window
	XSetWindowAttributes attributes;
	attributes.background_pixel = BlackPixel(display, screen);
	attributes.event_mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask
			| KeyReleaseMask | ButtonMotionMask | PointerMotionHintMask
			| StructureNotifyMask | PointerMotionMask | PointerMotionHintMask/*| ExposureMask*/;
	unsigned long valuemask = CWBackPixel | CWEventMask;
	if (!framed) {
		attributes.override_redirect = 1;
		valuemask |= CWOverrideRedirect;
	}
	win = XCreateWindow(display, RootWindow(display, screen), 50, 50, bounds.getWidth(),
			bounds.getHeight(), CopyFromParent, CopyFromParent, InputOutput, visual,
			valuemask, &attributes);
	// register win
	winmap[win] = this;
	// participate in the window manager 'delete yourself' protocol
	if (XSetWMProtocols(display, win, &wm_delete_window_atom, 1)==0) {
		SAMBAG_ASSERT(false);
	}
	// disco stuff
	createSurface();
}
//-----------------------------------------------------------------------------
void X11WindowImpl::destroyWindow() {
	// unregister window
	winmap.erase(win);
	// X11's destroy
	XDestroyWindow(display, win);
	XSync(display, 0);
	win = 0;
	--instances;
}
//-----------------------------------------------------------------------------
void X11WindowImpl::close() {
	invokeLater(DestroyWindow::create(this));
	visible = false;
}
//-----------------------------------------------------------------------------
X11WindowImpl::~X11WindowImpl() {
	destroyWindow();
}
//-----------------------------------------------------------------------------
bool X11WindowImpl::isVisible() const {
	return visible;
}
//-----------------------------------------------------------------------------
void X11WindowImpl::show() {
	// pop up the window
	XMapWindow(display, win);
	XSync(display, win);
	visible = true;
}
//-----------------------------------------------------------------------------
Rectangle X11WindowImpl::getBounds() const {
	return bounds;
}
//-----------------------------------------------------------------------------
void X11WindowImpl::createSurface() {
	using namespace sambag;
	Rectangle r = getBounds();
	surface = disco::X11Surface::create(display, win, visual,
			r.getWidth(), r.getHeight());
}
//-----------------------------------------------------------------------------
void X11WindowImpl::setBounds(const Rectangle &r) {
	bounds = r;
	if (!isVisible())
		return;
	Rectangle curr = getBounds();
	if (curr == r)
		return;
	typedef unsigned int UI;
	XMoveResizeWindow(display, win, (int)r.x0().x(),
			(int)r.x0().y(),
			(UI)r.getWidth(),
			(UI)r.getHeight()
	);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::mainLoop() {
	// TODO: sync with destroyWindow
	while (instances>0) {
		// read in and process all pending events for the main window
		XEvent event;
		while (instances>0 && XPending(display)) {
			XNextEvent(display, &event);
			handleEvent(event);
		}
		//processDraw();
		processInvocations();
		microsleep(1000);
		XSync(display, 0);
	}
	XCloseDisplay(display);
	display = NULL;

}
//-----------------------------------------------------------------------------
void X11WindowImpl::setTitle(const std::string &title) {
	X11WindowImpl::title = title;
	if (!isVisible())
		return;
	// set the window title
	XTextProperty window_name;
	window_name.value = (unsigned char*) title.c_str();
	window_name.encoding = XA_STRING;
	window_name.format = 8;
	window_name.nitems = strlen((char *) window_name.value);
	XSetWMName(display, win, &window_name);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::processInvocations() {
	using namespace sambag::com;
	Invocations::iterator it = invocations.begin();
	while (it!=invocations.end()) {
		(*it)->execute();
		it = invocations.erase(it);
	}
}
//-----------------------------------------------------------------------------
void X11WindowImpl::invokeLater(sambag::com::ICommand::Ptr cmd) {
	invocations.push_back(cmd);
}
//-----------------------------------------------------------------------------
void X11WindowImpl::handleEvent(XEvent &event) {
	static int mx = 0, my = 0; // mouse position
	Window win = event.xany.window;
	X11WindowImpl *src = getX11WindowImpl(win);
	if (!src)
		return;
	switch (event.type) {
	case ButtonPress: {
		int buttons = 0;
		if (event.xbutton.button == Button1)
			buttons |= 1;
		if (event.xbutton.button == Button2)
			buttons |= 2;
		if (event.xbutton.button == Button3)
			buttons |= 4;
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
			buttons |= 2;
		if (event.xbutton.button == Button3)
			buttons |= 4;
		mx = event.xbutton.x;
		my = event.xbutton.y;
		src->handleMouseButtonReleaseEvent(mx, my, buttons);
	}
		return;

	case MotionNotify: {
		if (event.xmotion.is_hint) {
			Window root, child;
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

	case ConfigureNotify:
		//width = event.xconfigure.width;
		//height = event.xconfigure.height;
		return;
	}
}
//-----------------------------------------------------------------------------
void X11WindowImpl::
handleMouseButtonPressEvent(int x, int y, int buttons)
{
}
//-----------------------------------------------------------------------------
void X11WindowImpl::
handleMouseButtonReleaseEvent(int x, int y, int buttons)
{

}
//-----------------------------------------------------------------------------
void X11WindowImpl::handleMouseMotionEvent(int x, int y) {

}
//-----------------------------------------------------------------------------
void X11WindowImpl::startMainLoop() {
	mainLoop();
}
//=============================================================================
// class X11Window
//=============================================================================
X11Window::X11Window(AWindow::Ptr parent) : AWindow(parent) {
	X11WindowImpl::createWindow(false);
}
//-----------------------------------------------------------------------------
void X11Window::setBounds(const Rectangle &r) {
	X11WindowImpl::setBounds(r);
}
//-----------------------------------------------------------------------------
Rectangle X11Window::getBounds() const {
	return X11WindowImpl::getBounds();
}
//=============================================================================
// class X11FramedWindow
//=============================================================================
X11FramedWindow::X11FramedWindow(AWindow::Ptr parent) :
		AFramedWindow(parent)
{
	X11WindowImpl::createWindow(true);
}
//-----------------------------------------------------------------------------
void X11FramedWindow::setBounds(const Rectangle &r) {
	X11WindowImpl::setBounds(r);
}
//-----------------------------------------------------------------------------
Rectangle X11FramedWindow::getBounds() const {
	return X11WindowImpl::getBounds();
}
//-----------------------------------------------------------------------------
void X11FramedWindow::setTitle(const std::string &title) {
	X11WindowImpl::setTitle(title);
}
}} // namespace(s)

#endif // DISCO_USE_X11
