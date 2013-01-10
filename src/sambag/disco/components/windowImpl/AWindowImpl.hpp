/*
 * AWindowImpl.hpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_AWINDOW_H
#define SAMBAG_AWINDOW_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/com/events/Events.hpp>
#include "WindowFlags.hpp"
#include <string>
#include <sambag/disco/components/events/MouseEvent.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
struct OnOpenEvent {
//=============================================================================
	// implement Window source ptr. is quite tricky
	// because the Window Object is possible gone on windowimpl.
	// closing event.
};
//=============================================================================
struct OnCloseEvent {
//=============================================================================
	// implement Window source ptr. is quite tricky
	// because the Window Object is possible gone on windowimpl.
	// closing event.
};
//=============================================================================
struct OnBoundsChanged {
//=============================================================================
	Rectangle newBounds;
	const Rectangle & getNewBounds() const { return newBounds; }
	OnBoundsChanged(const Rectangle &r) : newBounds(r) {}
};
//=============================================================================
/** 
  * @class AWindowImpl.
  */
class AWindowImpl :
	public sambag::com::events::EventSender<OnOpenEvent>,
	public sambag::com::events::EventSender<OnCloseEvent>,
	public sambag::com::events::EventSender<OnBoundsChanged> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AWindowImpl> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<AWindowImpl> WPtr;
protected:
private:
public:
	//-------------------------------------------------------------------------
	virtual EventSender<events::MouseEvent> * getMouseEventSender() = 0;
	//-------------------------------------------------------------------------
	virtual void open() = 0;
	//-------------------------------------------------------------------------
	virtual void close() = 0;
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r) = 0;
	//-------------------------------------------------------------------------
	/**
	 * @return a system dependent pointer:
	 *     win32: HWND,
	 *     ...
	 */
	virtual void * getSystemHandle() = 0;
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
	virtual Rectangle getBounds() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * If this window is nested in another (the host) this function return it's
	 * bounds.
	 * @return bounds of host window. 
	 * @see WindowToolkit::createNestedWindow()
	 */
	virtual Rectangle getHostBounds() const = 0;
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
	virtual void invalidateWindow(const Rectangle &area = NULL_RECTANGLE) = 0;
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root) = 0;
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr parent) = 0;
	//-------------------------------------------------------------------------
	virtual void setFlag(WindowFlags::Flag flag, bool val) = 0;
	//-------------------------------------------------------------------------
	virtual bool getFlag(WindowFlags::Flag flag) const = 0;
	//-------------------------------------------------------------------------
	virtual bool isVisible() const = 0;
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title) = 0;
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const = 0;

}; // AWindowImpl
}}} // namespace(s)
#endif /* SAMBAG_AWINDOW_H */
