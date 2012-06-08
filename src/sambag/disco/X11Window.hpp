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
#include "IDiscoFactory.hpp"
#include "components/RedrawManager.hpp"
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
struct OpenWindow : public sambag::com::ICommand {
//=============================================================================
	typedef boost::shared_ptr<OpenWindow> Ptr;
	X11WindowImpl * dst;
	virtual void execute();
	static Ptr create(X11WindowImpl* _dst) {
		Ptr res(new OpenWindow());
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
friend struct OpenWindow;
private:
	//-------------------------------------------------------------------------
	bool framed;
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
	X11WindowImpl(bool framed);
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Window, X11WindowImpl*> WinMap;
	//-------------------------------------------------------------------------
	static WinMap winmap;
	//-------------------------------------------------------------------------
	// X11
	static Display *display;
	static int instances;
	int screen;
	static Atom wm_protocols_atom;
	static Atom wm_delete_window_atom;
	Visual *visual;
	Window win;
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
	virtual void onDestroyed() = 0;
	//-------------------------------------------------------------------------
	virtual void boundsUpdated() = 0;
	//-------------------------------------------------------------------------
	virtual void processDraw() = 0;
public:
	//-------------------------------------------------------------------------
	bool isVisible() const;
	//-------------------------------------------------------------------------
	void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	static void invokeLater(sambag::com::ICommand::Ptr cmd);
	//-------------------------------------------------------------------------
	void open();
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
template <class WindowBase>
class X11Window : public WindowBase, private X11WindowImpl {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<X11Window> Ptr;
protected:
	//-------------------------------------------------------------------------
	sambag::disco::IImageSurface::Ptr bff;
	//-------------------------------------------------------------------------
	X11Window(AWindow::Ptr parent, bool framed) :
				WindowBase(parent),
				X11WindowImpl(framed)
	{
	}
	//-------------------------------------------------------------------------
	virtual void processDraw();
	//-------------------------------------------------------------------------
	virtual void onCreated();
	//-------------------------------------------------------------------------
	virtual void onDestroyed();
private:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
	//-------------------------------------------------------------------------
	components::events::MouseEventCreator::Ptr mec;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	void handleMouseButtonPressEvent(int x, int y, int buttons) {
		if (!mec)
			return;
		mec->createPressEvent(x,y,buttons);
	}
	//-------------------------------------------------------------------------
	void handleMouseButtonReleaseEvent(int x, int y, int buttons) {
		if (!mec)
			return;
		mec->createReleaseEvent(x,y,buttons);
	}
	//-------------------------------------------------------------------------
	void handleMouseMotionEvent(int x, int y) {
		if (!mec)
			return;
		mec->createMoveEvent(x,y);
	}
	//-------------------------------------------------------------------------
	void update() {
		needUpdate = true;
	}
	//-------------------------------------------------------------------------
	virtual void boundsUpdated();
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &r) {
		X11WindowImpl::setBounds(r);
	}
	//-------------------------------------------------------------------------
	Rectangle getBounds() const {
		return X11WindowImpl::getBounds();
	}
	//-------------------------------------------------------------------------
	static Ptr create(AWindow::Ptr parent, bool framed) {
		Ptr res(new X11Window(parent, framed));
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void open() {
		// init surface
		X11WindowImpl::open();
	}
	//-------------------------------------------------------------------------
	virtual void close() {
		WindowBase::rootPane->setSurface(ISurface::Ptr());
		mec.reset();
		X11WindowImpl::close();
	}
	//--------------------------------------------------------------------------
	virtual void setTitle(const std::string &title) {
		X11WindowImpl::setTitle(title);
	}
}; // X11Window
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class WindowBase>
void X11Window<WindowBase>::onCreated() {
	using namespace components;
	// create offbuffer
	Dimension dim = X11WindowImpl::getBounds().getDimension();
	bff = sambag::disco::
			getDiscoFactory()->createImageSurface(dim.width(), dim.height());
	// create root pane
	WindowBase::rootPane->setSurface(bff);
	WindowBase::rootPane->setBounds(X11WindowImpl::getBounds());
	// create mousevent creator
	mec = events::MouseEventCreator::create(WindowBase::rootPane);
	update();
}
//-----------------------------------------------------------------------------
template <class WindowBase>
void X11Window<WindowBase>::onDestroyed() {
}
//-----------------------------------------------------------------------------
template <class WindowBase>
void X11Window<WindowBase>::processDraw() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	RootPane::Ptr root = WindowBase::rootPane;
	IDrawContext::Ptr cn = getDiscoFactory()->createContext(surface);
	if (needUpdate) {
		root->draw(root->getDrawContext());
		needUpdate=false;
		return;
	}
	RedrawManager::currentManager(root)->drawDirtyRegions();
	cn->drawSurface(bff);
}
//-----------------------------------------------------------------------------
template <class WindowBase>
void X11Window<WindowBase>::boundsUpdated() {
	using namespace components;
	// create offbuffer
	Dimension dim = X11WindowImpl::getBounds().getDimension();
	if (dim == WindowBase::rootPane->getSize())
		return;
	std::cout<<dim<<std::endl;
	bff = sambag::disco::
			getDiscoFactory()->createImageSurface(dim.width(), dim.height());
	// create root pane
	WindowBase::rootPane->setSurface(bff);
	WindowBase::rootPane->setSize(dim);
	WindowBase::rootPane->validate();
	RedrawManager::currentManager(WindowBase::rootPane)->
			markCompletelyDirty(WindowBase::rootPane);
	update();
}
}} // namespace(s)
#endif //DISCO_USE_X11
#endif /* SAMBAG_X11WINDOW_H */
