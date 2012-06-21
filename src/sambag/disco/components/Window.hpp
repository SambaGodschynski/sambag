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

#define SAMBAG_STD_WINDOW_CREATOR(window_class_name) 						\
	Ptr getPtr() const {													\
		return 																\
		boost::shared_dynamic_cast<window_class_name>(Window::getPtr()); 	\
	}																		\
	static Ptr create(Window::Ptr parent = Window::Ptr()) { 				\
		Ptr neu(new window_class_name(parent));								\
		neu->self = neu;													\
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
	typedef boost::shared_ptr<Window> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Window> WPtr;
protected:
	//-------------------------------------------------------------------------
	Window::Ptr parent;
	//-------------------------------------------------------------------------
	components::RootPane::Ptr rootPane;
	//-------------------------------------------------------------------------
	Window(Window::Ptr parent);
	//-------------------------------------------------------------------------
	AWindowImpl::Ptr windowImpl;
	//-------------------------------------------------------------------------
	/**
	 * has to call from create()
	 */
	virtual void initWindow();
private:
	//-------------------------------------------------------------------------
	void onParentRemove(void *src, const OnCloseEvent &ev);
	//-------------------------------------------------------------------------
	void onBoundsChanged(void *src, const OnBoundsChanged &ev);
public:
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
	Rectangle getWindowBounds() const;
	//-------------------------------------------------------------------------
	Point2D getWindowLocation() const {
		return getWindowBounds().x0();
	}
	//-------------------------------------------------------------------------
	Dimension getWindowSize() const {
		return getWindowBounds().getDimension();
	}
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
	//-------------------------------------------------------------------------
	/**
	 * Sets the minimum size of this window to a constant value.
	 * Subsequent calls to getMinimumSize will always return this value.
	 * If current window's size is less than minimumSize the size of the
	 * window is automatically enlarged to honor the minimum size.
	 */
	virtual void pack();
	//-------------------------------------------------------------------------
	static Ptr create(Window::Ptr parent=WindowPtr()) {
		Ptr res(new Window(parent));
		res->self = res;
		res->initWindow();
		return res;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<Window>(self.lock());
	}
	//-------------------------------------------------------------------------
	components::RootPane::Ptr getRootPane() const;
	//-------------------------------------------------------------------------
	virtual void open();
	//-------------------------------------------------------------------------
	virtual void close();
	//-------------------------------------------------------------------------
	virtual void setWindowBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual void setWindowSize(const Dimension &d) {
		Rectangle neu = getWindowBounds();
		neu.setWidth(d.width());
		neu.setHeight(d.height());
		setWindowBounds(neu);
	}
	//-------------------------------------------------------------------------
	virtual void setWindowLocation(const Point2D &p) {
		Rectangle neu = getWindowBounds();
		neu.translate(p);
		setWindowBounds(neu);
	}
	//-------------------------------------------------------------------------
	virtual bool isVisible() const {
		if (!windowImpl)
			return false;
		return windowImpl->isVisible();
	}
}; // Window
}}} // namespace(s)
#endif /* SAMBAG_AWINDOW_H */
