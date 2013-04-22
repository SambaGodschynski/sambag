/*
 * _CocoaWindowImpl.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG__COCOAWINDOWIMPL_H
#define SAMBAG__COCOAWINDOWIMPL_H
#ifdef DISCO_USE_COCOA

#include <ApplicationServices/ApplicationServices.h>
#include "AutoReleasePool.h"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/**
 * @class _CocoaWindowImpl.
 * Encapsulated cocoa window implementation.
 */
class _CocoaWindowImpl {
public:
	//-------------------------------------------------------------------------
	typedef void* RawDiscoWindow;
	typedef void* RawDiscoView;
private:
	//-------------------------------------------------------------------------
	RawDiscoWindow window;
	RawDiscoView view;
	AutoReleasePool ap;
public:
	//-------------------------------------------------------------------------
	RawDiscoWindow getRawDiscoWindow() const { return window; }
	//-------------------------------------------------------------------------
	RawDiscoView getRawDiscoView() const { return view; }
	//-------------------------------------------------------------------------
	_CocoaWindowImpl();
	//-------------------------------------------------------------------------
	virtual ~_CocoaWindowImpl() {}
	//-------------------------------------------------------------------------
	static void startMainApp();
	//-------------------------------------------------------------------------
	void openWindow(_CocoaWindowImpl*, int x, int y, int w, int h);
	//-------------------------------------------------------------------------
	void closeWindow();
	//-------------------------------------------------------------------------
	void setBounds(int x, int y, int w, int h);
	//-------------------------------------------------------------------------
	void invalidateWindow(double x, double y, double w, double h);
	//-------------------------------------------------------------------------
	/**
	 * called by window delegate.
	 */
	void __windowDidResized();
	///////////////////////////////////////////////////////////////////////////
	// pseudo callbacks ( _CocoaWindowImpl => CocoaWindowImpl)
	//-------------------------------------------------------------------------
	virtual void __boundsChanged(int x, int y, int w, int h) = 0;
	//-------------------------------------------------------------------------
	virtual void __processDraw(CGContextRef context, int x, int y, int w, int h) = 0;
	//-------------------------------------------------------------------------
	virtual void __handleMouseButtonPressEvent(int x, int y, int buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void __handleMouseButtonReleaseEvent(int x, int y, int buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void __handleMouseMotionEvent(int x, int y) = 0;
};

}}}

#endif //DISCO_USE_COCOA
#endif //SAMBAG__COCOAWINDOWIMPL_H
