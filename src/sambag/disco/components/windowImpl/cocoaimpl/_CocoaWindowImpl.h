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
#include <string>
#include <sambag/disco/components/windowImpl/WindowFlags.hpp>

namespace cocoaImplTypes {
	typedef double Number;
}

namespace sambag { namespace disco { namespace components {
//=============================================================================
/**
 * @class _CocoaWindowImpl.
 * Encapsulated cocoa window implementation.
 */
class _CocoaWindowImpl : public WindowFlags {
public:
	//-------------------------------------------------------------------------
	typedef WindowFlags Super;
	typedef void* RawDiscoWindow;
	typedef void* RawDiscoView;
	typedef cocoaImplTypes::Number Number;
private:
	//-------------------------------------------------------------------------
	RawDiscoWindow window;
	RawDiscoView view;
	AutoReleasePool ap;
	//-------------------------------------------------------------------------
	/**
	 * creates NSWindow window style mask via Super::WindowFlags.
	 */
	int getWindowStyleMask() const;
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
	void openWindow(_CocoaWindowImpl*, Number x, Number y, Number w, Number h);
	//-------------------------------------------------------------------------
	void openNested(WindowRef, Number x, Number y, Number w, Number h);
	//-------------------------------------------------------------------------
	void closeWindow();
	//-------------------------------------------------------------------------
	std::string getTitle() const;
	//-------------------------------------------------------------------------
	void setTitle(const std::string &str);
	//-------------------------------------------------------------------------
	void setBounds(Number x, Number y, Number w, Number h);
	//-------------------------------------------------------------------------
	void invalidateWindow(Number x, Number y, Number w, Number h);
	//-------------------------------------------------------------------------
	WindowRef getWindowRef() const;
	//-------------------------------------------------------------------------
	/**
	 * called by window delegate.
	 */
	void ____windowBoundsChanged();
	///////////////////////////////////////////////////////////////////////////
	// pseudo callbacks ( _CocoaWindowImpl => CocoaWindowImpl)
	//-------------------------------------------------------------------------
	virtual void __boundsChanged(Number x, Number y, Number w, Number h) = 0;
	//-------------------------------------------------------------------------
	virtual void __processDraw(CGContextRef context, Number x, Number y, Number w, Number h) = 0;
	//-------------------------------------------------------------------------
	virtual void __handleMouseButtonPressEvent(Number x, Number y, Number buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void __handleMouseButtonReleaseEvent(Number x, Number y, Number buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void __handleMouseMotionEvent(Number x, Number y) = 0;
};
//=============================================================================
/**
 * @class _CocoaToolkitImpl impl.
 * Encapsulated cocoa window toolkit implementation.
 */
class _CocoaToolkitImpl {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef cocoaImplTypes::Number Number;
	//-------------------------------------------------------------------------
	static void startMainApp();
	//-------------------------------------------------------------------------
	static void initMainApp();
	//-------------------------------------------------------------------------
	static void getScreenDimension(Number &outWidth, Number &outHeight);
};
}}}

#endif //DISCO_USE_COCOA
#endif //SAMBAG__COCOAWINDOWIMPL_H
