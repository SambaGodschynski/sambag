/*
 * AWindow.hpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WINDOW_H
#define SAMBAG_WINDOW_H

#include <boost/shared_ptr.hpp>
#include "components/RootPane.hpp"
#include "ISurface.hpp"
#include "windowImpl/AWindow.hpp"
#include "Forward.hpp"
#include <sambag/com/events/Events.hpp>

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class Window.
  */
class Window {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Window> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Window> WPtr;
protected:
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	Window::Ptr parent;
	//-------------------------------------------------------------------------
	components::RootPane::Ptr rootPane;
	//-------------------------------------------------------------------------
	Window(Window::Ptr parent);
	//-------------------------------------------------------------------------
	AWindow::Ptr windowImpl;
private:
	//-------------------------------------------------------------------------
	void onParentRemove(void *src, const OnCloseEvent &ev);
public:
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
	static void startMainLoop();
	//-------------------------------------------------------------------------
	static Ptr create(Window::Ptr parent=WindowPtr()) {
		Ptr res(new Window(parent));
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	components::RootPane::Ptr getRootPane() const;
	//-------------------------------------------------------------------------
	virtual void open();
	//-------------------------------------------------------------------------
	virtual void close();
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual void setSize(const Dimension &d) {
		Rectangle neu = getBounds();
		neu.setWidth(d.width());
		neu.setHeight(d.height());
		setBounds(neu);
	}
	//-------------------------------------------------------------------------
	virtual void setLocation(const Point2D &p) {
		Rectangle neu = getBounds();
		neu.translate(p);
		setBounds(neu);
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	virtual Dimension getSize() const {
		Rectangle curr = getBounds();
		return curr.getDimension();
	}
	//-------------------------------------------------------------------------
	virtual Point2D getLocation() const {
		Rectangle curr = getBounds();
		return curr.x0();
	}
	//-------------------------------------------------------------------------
	virtual bool isVisible() const {
		if (!windowImpl)
			return false;
		return windowImpl->isVisible();
	}
}; // Window
}} // namespace(s)
#endif /* SAMBAG_AWINDOW_H */
