/*
 * AWindow.hpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_AWINDOW_H
#define SAMBAG_AWINDOW_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/Forward.hpp>

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
protected:
private:
public:
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
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root) = 0;
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr parent) = 0;
	//-------------------------------------------------------------------------
	virtual void setFramed(bool b) = 0;
	//-------------------------------------------------------------------------
	virtual bool isVisible() const = 0;

}; // AWindow
}} // namespace(s)
#endif /* SAMBAG_AWINDOW_H */
