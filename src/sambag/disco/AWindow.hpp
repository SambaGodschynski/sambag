/*
 * AWindow.hpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_AWINDOW_H
#define SAMBAG_AWINDOW_H

#include <boost/shared_ptr.hpp>
#include "components/RootPane.hpp"
#include "ISurface.hpp"

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class AWindow.
  */
class AWindow {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AWindow> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<AWindow> WPtr;
protected:
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	AWindow::Ptr parent;
	//-------------------------------------------------------------------------
	components::RootPane::Ptr rootPane;
	//-------------------------------------------------------------------------
	AWindow(AWindow::Ptr parent);
	//-------------------------------------------------------------------------
	virtual void init(ISurface::Ptr surf);
private:
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	components::RootPane::Ptr getRootPane() const;
	//-------------------------------------------------------------------------
	virtual void open() = 0;
	//-------------------------------------------------------------------------
	virtual void close() = 0;
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r) = 0;
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
	virtual Dimension getSize() const {
		Rectangle curr = getBounds();
		return curr.getDimension();
	}
	//-------------------------------------------------------------------------
	virtual Point2D getLocation() const {
		Rectangle curr = getBounds();
		return curr.x0();
	}
}; // AWindow
}} // namespace(s)
#endif /* SAMBAG_AWINDOW_H */
