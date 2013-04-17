/*
 * X11Window.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COCOAWINDOW_H
#define SAMBAG_COCOAWINDOW_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_map.hpp>

#ifdef DISCO_USE_COCOA
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/ISurface.hpp>
#include "AWindowImpl.hpp"
#include "WindowFlags.hpp"
#include <sambag/com/ArbitraryType.hpp>
#include <boost/tuple/tuple.hpp>
#include "cocoaimpl/_CocoaWindowImpl.h"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class CocoaWindowImpl.
  */
class CocoaWindowImpl :
	public WindowFlags,
	public _CocoaWindowImpl // encapsulated objective-c impl.
{
//=============================================================================
friend class CocoaWindowToolkit;
public:
	//-------------------------------------------------------------------------
	typedef _CocoaWindowImpl Impl;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CocoaWindowImpl> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<CocoaWindowImpl> WPtr;
private:
public:
private:
	//-------------------------------------------------------------------------
	bool visible;
	//-------------------------------------------------------------------------
	Rectangle bounds;
	//-------------------------------------------------------------------------
	std::string title;
	//-------------------------------------------------------------------------
	void updateBoundsToWindow();
	//-------------------------------------------------------------------------
	void updateWindowToBounds(const Rectangle &r);
protected:
	//-------------------------------------------------------------------------
	void initAsNestedWindow(ArbitraryType::Ptr osParent, const Rectangle &area);
	//-------------------------------------------------------------------------
	WPtr self; // setted during WindowImpl::create()
	//-------------------------------------------------------------------------
	CocoaWindowImpl();
	//-------------------------------------------------------------------------
	// Impl. by WindowImpl
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
	virtual void processDraw(sambag::disco::ISurface::Ptr surface) = 0;
	//-------------------------------------------------------------------------
	void _open(AWindowImplPtr parent);
	//-------------------------------------------------------------------------
	void _close();
public:
	//-------------------------------------------------------------------------
	virtual void __processDraw(CGContextRef context, int x, int y, int w, int h);
	//-------------------------------------------------------------------------
	static void startMainApp();
	//-------------------------------------------------------------------------
	void invalidateWindow(const Rectangle &area = NULL_RECTANGLE);
	//-------------------------------------------------------------------------
	void * getSystemHandle() const;
	//-------------------------------------------------------------------------
	Ptr getPtr() {
		return self.lock();
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
	virtual ~CocoaWindowImpl();
	//-------------------------------------------------------------------------
	Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	Rectangle getHostBounds() const;
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &d);
}; // CocoaWindowImpl
}}}

#endif //DISCO_USE_COCOA
#endif /* SAMBAG_COCOAWINDOW_H */
