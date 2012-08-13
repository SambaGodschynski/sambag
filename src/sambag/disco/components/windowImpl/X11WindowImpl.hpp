/*
 * X11Window.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_X11WINDOW_H
#define SAMBAG_X11WINDOW_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#ifdef DISCO_USE_X11
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/ISurface.hpp>
#include "AWindowImpl.hpp"
#include "WindowFlags.hpp"

namespace sambag { namespace disco { namespace components {
class X11WindowToolkit;
class X11WindowImpl; typedef boost::shared_ptr<X11WindowImpl> X11WindowImplPtr;
//=============================================================================
/** 
  * @class X11Window.
  */
class X11WindowImpl : public WindowFlags {
//=============================================================================
friend struct DestroyWindow;
friend struct OpenWindow;
friend class X11WindowToolkit;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<X11WindowImpl> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<X11WindowImpl> WPtr;
private:
	//-------------------------------------------------------------------------
	Rectangle bounds;
	//-------------------------------------------------------------------------
	std::string title;
	//-------------------------------------------------------------------------
	void createSurface();
	//-------------------------------------------------------------------------
	static void handleEvent(XEvent &event);
	//-------------------------------------------------------------------------
	void destroyWindow();
	//-------------------------------------------------------------------------
	static X11WindowImpl * getX11WindowImpl(::Window win);
	//-------------------------------------------------------------------------
	void createWindow();
	//-------------------------------------------------------------------------
	void updateBoundsToWindow();
	//-------------------------------------------------------------------------
	void updateWindowToBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	void updateTitle();
	//-------------------------------------------------------------------------
	bool visible;
	//-------------------------------------------------------------------------
	static void drawAll();
protected:
	//-------------------------------------------------------------------------
	WPtr self; // setted during WindowImpl::create()
	//-------------------------------------------------------------------------
	X11WindowImpl();
	//-------------------------------------------------------------------------
	typedef boost::unordered_map< ::Window, X11WindowImpl* > WinMap;
	//-------------------------------------------------------------------------
	static WinMap winmap;
	//-------------------------------------------------------------------------
	// X11
	static int instances;
	int screen;
	::Display *display;
	static Atom wm_protocols_atom;
	static Atom wm_delete_window_atom;
	Visual *visual;
	::Window win;
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
	//-------------------------------------------------------------------------
	virtual void handleMouseButtonPressEvent(int x, int y, int buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void handleMouseButtonReleaseEvent(int x, int y, int buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void handleMouseMotionEvent(int x, int y) = 0;
	//-------------------------------------------------------------------------
	virtual void onCreated() = 0;
	//-------------------------------------------------------------------------
	virtual void onDestroy() = 0;
	//-------------------------------------------------------------------------
	virtual void boundsUpdated() = 0;
	//-------------------------------------------------------------------------
	virtual void processDraw() = 0;
public:
	//-------------------------------------------------------------------------
	static int getNumInstances() {
		return instances;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	bool isVisible() const;
	//-------------------------------------------------------------------------
	void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	std::string getTitle() const;
	//-------------------------------------------------------------------------
	void open(AWindowImplPtr parent);
	//-------------------------------------------------------------------------
	void close();
	//-------------------------------------------------------------------------
	virtual ~X11WindowImpl();
	//-------------------------------------------------------------------------
	Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &d);
}; // X11Window
}}}

#endif //DISCO_USE_X11
#endif /* SAMBAG_X11WINDOW_H */
