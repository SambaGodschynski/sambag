/*
 * X11Window.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_X11WINDOW_H
#define SAMBAG_X11WINDOW_H

#include <boost/shared_ptr.hpp>
#include "AWindow.hpp"
#include "AFramedWindow.hpp"
#include <boost/unordered_map.hpp>

#ifdef DISCO_USE_X11
#include "X11Window.hpp"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include "components/events/MouseEventCreator.hpp"
#include <sambag/com/ICommand.hpp>
#include <list>
namespace sambag { namespace disco {
class X11WindowImpl;

//=============================================================================
struct DestroyWindow : public sambag::com::ICommand {
//=============================================================================
	typedef boost::shared_ptr<DestroyWindow> Ptr;
	X11WindowImpl * dst;
	virtual void execute();
	static Ptr create(X11WindowImpl* _dst) {
		Ptr res(new DestroyWindow());
		res->dst = _dst;
		return res;
	}
};
//=============================================================================
/** 
  * @class X11Window.
  */
class X11WindowImpl {
//=============================================================================
friend struct DestroyWindow;
private:
	//-------------------------------------------------------------------------
	Rectangle bounds;
	//-------------------------------------------------------------------------
	std::string title;
	//-------------------------------------------------------------------------
	typedef std::list<sambag::com::ICommand::Ptr> Invocations;
	//-------------------------------------------------------------------------
	static Invocations invocations;
	//-------------------------------------------------------------------------
	void createSurface();
	//-------------------------------------------------------------------------
	static void mainLoop();
	//-------------------------------------------------------------------------
	static void handleEvent(XEvent &event);
	//-------------------------------------------------------------------------
	void destroyWindow();
	//-------------------------------------------------------------------------
	static void processInvocations();
	//-------------------------------------------------------------------------
	static X11WindowImpl * getX11WindowImpl(Window win);
	//-------------------------------------------------------------------------
	bool visible;
protected:
	//-------------------------------------------------------------------------
	X11WindowImpl();
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Window, X11WindowImpl*> WinMap;
	//-------------------------------------------------------------------------
	static WinMap winmap;
	//-------------------------------------------------------------------------
	// X11
	static Display *display;
	static int instances;
	static int screen;
	static Atom wm_protocols_atom;
	static Atom wm_delete_window_atom;
	static Visual *visual;
	Window win;
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
	//-------------------------------------------------------------------------
	components::events::MouseEventCreator::Ptr mec;
	//-------------------------------------------------------------------------
	void createWindow(bool framed);
	//-------------------------------------------------------------------------
	void handleMouseButtonPressEvent(int x, int y, int buttons);
	//-------------------------------------------------------------------------
	void handleMouseButtonReleaseEvent(int x, int y, int buttons);
	//-------------------------------------------------------------------------
	void handleMouseMotionEvent(int x, int y);
public:
	//-------------------------------------------------------------------------
	bool isVisible() const;
	//-------------------------------------------------------------------------
	void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	static void invokeLater(sambag::com::ICommand::Ptr cmd);
	//-------------------------------------------------------------------------
	void show();
	//-------------------------------------------------------------------------
	void close();
	//-------------------------------------------------------------------------
	virtual ~X11WindowImpl();
	//-------------------------------------------------------------------------
	Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &d);
	//-------------------------------------------------------------------------
	/**
	 * Starts X11's main loop.
	 * No return until the last window is closed.
	 */
	static void startMainLoop();
}; // X11Window

//=============================================================================
/**
  * @class X11Window.
  */
class X11Window : public AWindow, private X11WindowImpl {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<X11Window> Ptr;
protected:
	//-------------------------------------------------------------------------
	X11Window(AWindow::Ptr parent);
private:
public:
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	static Ptr create(AWindow::Ptr parent) {
		Ptr res(new X11Window(parent));
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void open() {
		X11WindowImpl::show();
	}
	//-------------------------------------------------------------------------
	virtual void close() {
		X11WindowImpl::close();
	}
}; // X11Window
//=============================================================================
/**
  * @class X11FramedWindow.
  */
class X11FramedWindow : public AFramedWindow, private X11WindowImpl {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<X11FramedWindow> Ptr;
protected:
	//-------------------------------------------------------------------------
	X11FramedWindow(AWindow::Ptr parent);
private:
public:
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	static Ptr create(AWindow::Ptr parent) {
		Ptr res(new X11FramedWindow(parent));
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void open() {
		X11WindowImpl::show();
	}
	//-------------------------------------------------------------------------
	virtual void close() {
		X11WindowImpl::close();
	}
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title);
}; // X11FramedWindow
}} // namespace(s)
#endif //DISCO_USE_X11
#endif /* SAMBAG_X11WINDOW_H */
