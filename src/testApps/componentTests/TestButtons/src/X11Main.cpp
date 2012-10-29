// main window and event handling for X11

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdbe.h>
#include <string>
#include <iostream>
#include <list>
#include "DiscoView.hpp"
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/X11Surface.hpp>
#include <sambag/com/Exception.hpp>


//#define SAMBAG_USE_DBL_BUFFER

// D.i.s.c.o
sambag::disco::X11Surface::Ptr discoSurface;


static struct {
	std::string title;
	int width, height;
} wParam;

// X11 display info
static Display *display = 0;
static int screen = 0;
static Atom wm_protocols_atom = 0;
static Atom wm_delete_window_atom = 0;
static Visual *visual = 0;

// window
static Window win = 0; // X11 window, 0 if not initialized
static int width = 0, height = 0; // window size

static int last_key_pressed = 0; // last key pressed in the window
static int run = 1; // 1 if simulation running

static void dsError(const std::string &str) {
	std::cout << str << std::endl;
}
static void message(const std::string &str) {
	std::cout << str << std::endl;
}
#ifdef SAMBAG_USE_DBL_BUFFER
static XdbeBackBuffer d_backBuf;
static void createDiscoSurface(Display *d, Visual *v, int w, int h) {
	using namespace sambag;
	try {
		discoSurface = disco::X11Surface::create(d, d_backBuf, v, w, h);
	} catch (const sambag::com::Exception &ex) {
		dsError(ex.what);
		throw ;
	}
}
#else
static void createDiscoSurface(Display *d, Visual *v, int w, int h) {
	using namespace sambag;
	try {
		discoSurface = disco::X11Surface::create(d, win, v, w, h);
	} catch (const sambag::com::Exception &ex) {
		dsError(ex.what);
		throw ;
	}
}
#endif
#ifdef SAMBAG_USE_DBL_BUFFER
static Visual * getVisual(XdbeScreenVisualInfo *info) {
	// Choosing the first one, seems that they have all perflevel of 0,
	// and the depth varies.
	XVisualInfo xvisinfo_templ;
	xvisinfo_templ.visualid = info->visinfo[0].visual; // We know there's at least one
	// As far as I know, screens are densely packed, so we can assume that if at least 1 exists, it's screen 0.
	xvisinfo_templ.screen = 0;
	xvisinfo_templ.depth = info->visinfo[0].depth;

	int matches;
	XVisualInfo *xvisinfo_match = XGetVisualInfo(display, VisualIDMask
			| VisualScreenMask | VisualDepthMask, &xvisinfo_templ, &matches);

	if (!xvisinfo_match || matches < 1) {
		dsError("Couldn't match a Visual with double buffering\n");
		return NULL;
	}

	/*
	 printf("%d supported visuals\n", info->count);
	 for (int i = 0; i < info->count; ++i) {
	 printf("visual %d/%d: id %d, depth %d, perf %d\n",
	 i, info->count,
	 info->visinfo[i].visual,
	 info->visinfo[i].depth,
	 info->visinfo[i].perflevel);
	 }
	 printf("We got xvisinfo: id: %d, screen %d, depth %d\n",
	 xvisinfo_match->visualid, xvisinfo_match->screen, xvisinfo_match->depth);
	 */

	// We can use Visual from the match
	return xvisinfo_match->visual;
}

static XdbeScreenVisualInfo * initBackBuffer() {
	int major, minor;
	if (!display) {
		dsError("no display for xdbe.");
		return NULL;
	}
	if (!XdbeQueryExtension(display, &major, &minor)) {
		dsError("no xdbe support!");
		return NULL;
	}
	int numScreens = 1;
	Drawable screens[] = { DefaultRootWindow(display) };
	XdbeScreenVisualInfo *info = XdbeGetVisualInfo(display, screens,
			&numScreens);
	if (!info || numScreens < 1 || info->count < 1) {
		dsError("no xdbe support!");
		return NULL;
	}
	return info;
}
#endif

static void setWindowTitle(const std::string &title, int fps=0) {
	std::stringstream ss;
	ss << title << ": " << fps << "fps";
	// set the window title
	XTextProperty window_name;
	window_name.value = (unsigned char*) ss.str().c_str();
	window_name.encoding = XA_STRING;
	window_name.format = 8;
	window_name.nitems = strlen((char *) window_name.value);
	XSetWMName(display, win, &window_name);
}

void fpsCallbackHandler(int fps) {
	setWindowTitle(wParam.title, fps);
}

static void createMainWindow() {
	display = XOpenDisplay(NULL);
	if (!display)
		dsError("can not open X11 display");
	screen = DefaultScreen(display);

#ifdef SAMBAG_USE_DBL_BUFFER
	XdbeScreenVisualInfo * info = initBackBuffer();
	// init visual
	visual = getVisual(info);
#else
	// init visual
	visual = XDefaultVisual(display, 0);
#endif

	// initialize variables
	win = 0;
	width = wParam.width;
	height = wParam.height;
	last_key_pressed = 0;

	// create the window
	XSetWindowAttributes attributes;
	attributes.background_pixel = BlackPixel(display, screen);
	attributes.override_redirect = 1; // <-- without titlebar, always on top
	attributes.event_mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask
			| KeyReleaseMask | ButtonMotionMask | PointerMotionHintMask
			| StructureNotifyMask | PointerMotionMask | PointerMotionHintMask/*| ExposureMask*/;
	win = XCreateWindow(display, RootWindow(display, screen), 50, 50, width,
			height, CopyFromParent, CopyFromParent, InputOutput, visual,
			CWBackPixel | CWEventMask /*| CWOverrideRedirect<--*/, &attributes);

	setWindowTitle(wParam.title);

	// participate in the window manager 'delete yourself' protocol
	wm_protocols_atom = XInternAtom(display, "WM_PROTOCOLS", False);
	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
	if (XSetWMProtocols(display, win, &wm_delete_window_atom, 1) == 0)
		dsError("XSetWMProtocols() call failed");

#ifdef SAMBAG_USE_DBL_BUFFER
	//create backbuffer
	d_backBuf = XdbeAllocateBackBufferName(display, win, XdbeBackground);
#endif

	// disco stuff
	createDiscoSurface(display, visual, width, height);

	// pop up the window
	XMapWindow(display, win);
	XSync(display, win);
	// register fps callback
	registerFPSCallbackHandler(&fpsCallbackHandler);
}

static void destroyMainWindow() {
	XDestroyWindow(display, win);
	XSync(display, 0);
	XCloseDisplay(display);
	display = 0;
	win = 0;
	message("bye.");
}


static void handleEvent(XEvent &event) {
	static int mx = 0, my = 0; // mouse position
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
		handleMouseButtonPressEvent(mx, my, buttons);
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
		handleMouseButtonReleaseEvent(mx, my, buttons);
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
		handleMouseMotionEvent(mx, my);
	}
		return;

	case KeyPress: {
		KeySym key;
		XLookupString(&event.xkey, NULL, 0, &key, 0);
		if ((event.xkey.state & ControlMask) == 0) {

		} else if (event.xkey.state & ControlMask) {
			switch (key) {
			}
			last_key_pressed = key; // a kludgy place to put this...
		}
		return;

		case KeyRelease:
		{
			// hmmmm...
		}
		return;

		case ClientMessage:
		if (event.xclient.message_type == wm_protocols_atom
				&& event.xclient.format == 32 && Atom(event.xclient.data.l[0])
				== wm_delete_window_atom) {
			run = 0;
			return;
		}
		return;

		case ConfigureNotify:
		width = event.xconfigure.width;
		height = event.xconfigure.height;
		return;
	}
	}
}


void microsleep(int usecs) {
	usleep(usecs);
}

void mainLoop() {

	run = 1;
	while (run) {
		// read in and process all pending events for the main window
		XEvent event;
		while (run && XPending(display)) {
			XNextEvent(display, &event);
			handleEvent(event);
		}
		processDraw();
		microsleep(1000);
#ifdef SAMBAG_USE_DBL_BUFFER
		XdbeSwapInfo swapInfo;
		swapInfo.swap_window = win;
		swapInfo.swap_action = XdbeBackground;
		// XdbeSwapBuffers returns true on success, we return 0 on success.
		XdbeSwapBuffers(display, &swapInfo, 1);
		XFlush(display);
#endif
		XSync(display, 0);
	}
	destroyMainWindow();
}

extern "C" void dsStop() {
	run = 0;
}


int main(int argc, char **argv) {
	init(wParam.width, wParam.height);
	// show window
	wParam.title = "D.I.S.C.O.";
	createMainWindow();
	createWindow(discoSurface);
	mainLoop();
}

