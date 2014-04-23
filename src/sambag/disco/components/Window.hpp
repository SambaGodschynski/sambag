/*
 * AWindowImpl.hpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WINDOW_H
#define SAMBAG_WINDOW_H

#include <boost/shared_ptr.hpp>
#include "RootPane.hpp"
#include <sambag/disco/ISurface.hpp>
#include "windowImpl/AWindowImpl.hpp"
#include "Forward.hpp"
#include <sambag/com/events/Events.hpp>
#include "AContainer.hpp"
#include <set>
#include <sambag/com/Thread.hpp>

#define SAMBAG_STD_WINDOW_CREATOR(window_class_name) 						\
	Ptr getPtr() const {													\
		return 																\
		  boost::dynamic_pointer_cast<window_class_name>                     \
            ( ::sambag::disco::components::Window::getPtr() ); 	            \
	}																		\
	static Ptr create(::sambag::disco::components::Window::Ptr parent =     \
	  ::sambag::disco::components::Window::Ptr())                           \
	{ 																		\
		Ptr neu(new window_class_name(parent));								\
		neu->self = neu;                                                    \
		neu->postConstructor();                                             \
		neu->initWindow();													\
		return neu;															\
	}                                                                       

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Window.
  * TODO: for embedded usage (vstwindow) create
  * a 'embedded'-window (e.g. AEffGuiEditor) subclass.
  */
class Window : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Window> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Window> WPtr;
	//-------------------------------------------------------------------------
	enum CloseOperation {DISPOSE_ON_CLOSE, EXIT_ON_CLOSE};
	//-------------------------------------------------------------------------
	typedef std::set<WPtr> Windows;
protected:
	//-------------------------------------------------------------------------
	WindowWPtr parentWindow;
	//-------------------------------------------------------------------------
	components::RootPane::Ptr rootPane;
	//-------------------------------------------------------------------------
	Window(Window::Ptr parentWindow);
	//-------------------------------------------------------------------------
	Window(AWindowImpl::Ptr windowImpl, Window::Ptr parentWindow=WindowPtr());
	//-------------------------------------------------------------------------
	AWindowImpl::Ptr windowImpl;
	//-------------------------------------------------------------------------
	/**
	 * has to call from create()
	 */
	virtual void initWindow();
private:
	//-------------------------------------------------------------------------
	void onWindowImplClose(void *src, const OnCloseEvent &ev);
	void onWindowImplOpen(void *src, const OnOpenEvent &ev);
	//-------------------------------------------------------------------------
	void onParentClose(void *src, const OnCloseEvent &ev);
	//-------------------------------------------------------------------------
	void onBoundsChanged(void *src, const OnBoundsChanged &ev);
	//-------------------------------------------------------------------------
	static Windows openWindows;
public:
    //-------------------------------------------------------------------------
    /**
     * @return the Id of the Window thread.
     */
    com::ThreadId getThreadId() const;
	//-------------------------------------------------------------------------
	static const Windows & getOpenWindows() {
		return openWindows;
	}
	//-------------------------------------------------------------------------
	void setDefaultCloseOperation(CloseOperation op);
	//-------------------------------------------------------------------------
	CloseOperation getDefaultCloseOperation() const;
	//-------------------------------------------------------------------------
    virtual ~Window();
	//-------------------------------------------------------------------------
	/**
	 * notifys the (system) window to invalidate the given area.
	 * @param the area to invalidate, can be NULL_RECTANGLE to
	 * invalidate the whole area.
	 */
	virtual void invalidateWindow(const Rectangle &area = NULL_RECTANGLE);
	//-------------------------------------------------------------------------
	AWindowImpl::Ptr getWindowImpl() const;
	//-------------------------------------------------------------------------
	static void startMainLoop();
	//-------------------------------------------------------------------------
	virtual AContainerPtr getRootContainer() const;
	//-------------------------------------------------------------------------
	virtual Point2D getLocationOnScreen(const Point2D &p) const;
	//-------------------------------------------------------------------------
	virtual Point2D getLocationOnComponent(const Point2D &p) const;
	//-------------------------------------------------------------------------
	virtual AContainer::Ptr getParent() const {
		return AContainer::Ptr();
	}
	//-------------------------------------------------------------------------
	virtual WindowPtr getParentWindow() const {
		return parentWindow.lock();
	}
	//-------------------------------------------------------------------------
	Rectangle getWindowBounds() const;
	//-------------------------------------------------------------------------
	Point2D getWindowLocation() const {
		return getWindowBounds().x0();
	}
	//-------------------------------------------------------------------------
	Dimension getWindowSize() const {
		return getWindowBounds().getDimension();
	}
	/////////////
	// OnCloseEventSender
	//-------------------------------------------------------------------------
	typedef sambag::com::events::EventSender<OnCloseEvent> OnCloseEventSender;
	//-------------------------------------------------------------------------
	typedef sambag::com::events::AnyWPtr AnyWPtr;
	//-------------------------------------------------------------------------
	OnCloseEventSender::Connection
	addOnCloseEventListener(const OnCloseEventSender::EventFunction &f);
	//-------------------------------------------------------------------------
	OnCloseEventSender::Connection
	addTrackedOnCloseEventListener
	(const OnCloseEventSender::EventFunction &f, AnyWPtr ptr );
	/////////////
	// OnCloseEventSender
	//-------------------------------------------------------------------------
	typedef sambag::com::events::EventSender<OnOpenEvent> OnOpenEventSender;
	//-------------------------------------------------------------------------
	OnOpenEventSender::Connection
	addOnOpenEventListener(const OnOpenEventSender::EventFunction &f);
	//-------------------------------------------------------------------------
	OnOpenEventSender::Connection
	addTrackedOnOpenEventListener
	(const OnOpenEventSender::EventFunction &f, AnyWPtr ptr );
	/////////////
	// WindwowMouseEvent
	//-------------------------------------------------------------------------
	typedef sambag::com::events::EventSender<events::MouseEvent> WindwowMouseEvent;
	//-------------------------------------------------------------------------
	WindwowMouseEvent::Connection
	addWindowMouseEventListener(const WindwowMouseEvent::EventFunction &f);
	//-------------------------------------------------------------------------
	WindwowMouseEvent::Connection
	addTrackedWindowMouseEventListener
	(const WindwowMouseEvent::EventFunction &f, AnyWPtr ptr );
	//-------------------------------------------------------------------------
	/**
	 * Sets the minimum size of this window to a constant value.
	 * Subsequent calls to getMinimumSize will always return this value.
	 * If current window's size is less than minimumSize the size of the
	 * window is automatically enlarged to honor the minimum size.
	 */
	virtual void pack();
	//-------------------------------------------------------------------------
	static Ptr create(Window::Ptr parentWindow=WindowPtr()) {
		Ptr res(new Window(parentWindow));
		res->self = res;
        res->postConstructor();
		res->initWindow();
		return res;
	}
	//-------------------------------------------------------------------------
	static Ptr create(AWindowImpl::Ptr impl, Window::Ptr parentWindow=WindowPtr()) {
		Ptr res(new Window(impl, parentWindow));
		res->self = res;
        res->postConstructor();
		res->initWindow();
		return res;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::dynamic_pointer_cast<Window>(self.lock());
	}
	//-------------------------------------------------------------------------
	RootPane::Ptr getRootPane() const;
	//-------------------------------------------------------------------------
	Panel::Ptr getContentPane() const;
	//-------------------------------------------------------------------------
	virtual void open();
	//-------------------------------------------------------------------------
	virtual void close();
	//-------------------------------------------------------------------------
	virtual void setWindowBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual void setWindowSize(const Dimension &d);
	//-------------------------------------------------------------------------
	virtual void setWindowLocation(const Point2D &p);
	//-------------------------------------------------------------------------
	virtual bool isVisible() const;
	//-------------------------------------------------------------------------
	/**
	 * @return true if window is open.
	 * @see isVisible().
	 */
	virtual bool isOpen() const {
		return isVisible();
	}
	//-------------------------------------------------------------------------
	/**
	 * @override
	 */
	virtual void setEnabled(bool b);
	//-------------------------------------------------------------------------
	/**
	 * determines the optimal location of the window related to the given window.
	 * @param the window which is the base for the location determination.
	 *        If null, the parentwindow will be used.
	 */
	virtual void positionWindow(Window::Ptr win = Window::Ptr());
}; // Window
}}} // namespace(s)
#endif /* SAMBAG_AWINDOW_H */
