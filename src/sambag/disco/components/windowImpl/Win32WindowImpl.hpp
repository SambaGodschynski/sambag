/*
 * X11Window.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32WINDOW_H
#define SAMBAG_WIN32WINDOW_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#ifdef DISCO_USE_WIN32
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/ISurface.hpp>
#include <windows.h>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class X11Window.
  */
class Win32WindowImpl {
//=============================================================================
friend class Win32WindowToolkit;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Win32WindowImpl> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Win32WindowImpl> WPtr;
private:
	//-------------------------------------------------------------------------
	static LRESULT CALLBACK 
		wndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	//-------------------------------------------------------------------------
	static int instances; 
	//-------------------------------------------------------------------------
	HWND win;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> framed;
	//-------------------------------------------------------------------------
	Rectangle bounds;
	//-------------------------------------------------------------------------
	std::string title;
	//-------------------------------------------------------------------------
	void createSurface();
	//-------------------------------------------------------------------------
	void destroyWindow();
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
	void update();
	//-------------------------------------------------------------------------
	static Win32WindowImpl * getWin32WindowImpl(HWND win);
protected:
	//-------------------------------------------------------------------------
	WPtr self; // setted during WindowImpl::create()
	//-------------------------------------------------------------------------
	Win32WindowImpl();
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<HWND, Win32WindowImpl*> WinMap;
	//-------------------------------------------------------------------------
	static WinMap winmap;
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
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
	virtual void processDraw() = 0;
public:
	//-------------------------------------------------------------------------
	static int getNumInstances() {
		return instances;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	void setFramed(bool b) {
		framed = b;
	}
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	bool isVisible() const;
	//-------------------------------------------------------------------------
	void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	std::string getTitle() const;
	//-------------------------------------------------------------------------
	void open();
	//-------------------------------------------------------------------------
	void close();
	//-------------------------------------------------------------------------
	virtual ~Win32WindowImpl();
	//-------------------------------------------------------------------------
	Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &d);
}; // X11Window
}}}

#endif //DISCO_USE_X11
#endif /* SAMBAG_WIN32WINDOW_H */