/*
 * X11Window.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32WINDOW_H
#define SAMBAG_WIN32WINDOW_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_map.hpp>

#ifdef DISCO_USE_WIN32
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/ISurface.hpp>
#include "AWindowImpl.hpp"
#include "WindowFlags.hpp"
#include <windows.h>
#include <sambag/com/ArbitraryType.hpp>
#include <sambag/disco/Win32Surface.hpp>
#include <boost/tuple/tuple.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
struct WndClassManager {
//=============================================================================
	typedef boost::tuple<std::string, HINSTANCE, bool> WndClassId;
	struct WndClassHolder {
		typedef boost::shared_ptr<WndClassHolder> Ptr;
		typedef boost::weak_ptr<WndClassHolder> WPtr;
		WndClassId wndClassId;
		WNDCLASS wndClass;
		static Ptr create(const WndClassId &id);
		~WndClassHolder();
		const WNDCLASS & getWndClass() const {
			return wndClass;
		}
	};
	typedef std::map<WndClassId, WndClassHolder::Ptr> WndClassMap;
	static WndClassMap wndClassMap;
	static WndClassHolder::Ptr getWndClassHolder(
		const std::string &name, 
		HINSTANCE hi, 
		bool isNestedWindow = false);
};
//=============================================================================
/** 
  * @class X11Window.
  */
class Win32WindowImpl : public WindowFlags {
//=============================================================================
friend class Win32WindowToolkit;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Win32WindowImpl> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Win32WindowImpl> WPtr;
private:
	//-------------------------------------------------------------------------
	WndClassManager::WndClassHolder::Ptr wndClassHolder;
	//-------------------------------------------------------------------------
	void _open(AWindowImplPtr parent);
	//-------------------------------------------------------------------------
	void _close();
	//-------------------------------------------------------------------------
	void unregisterWindow();
	//-------------------------------------------------------------------------
public:
	static LRESULT CALLBACK 
		__wndProc_(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
private:
	//-------------------------------------------------------------------------
	static int instances; 
	//-------------------------------------------------------------------------
	HWND win;
	//-------------------------------------------------------------------------
	Rectangle bounds;
	//-------------------------------------------------------------------------
	std::string title;
	//-------------------------------------------------------------------------
	void invalidateSurface();
	//-------------------------------------------------------------------------
	void destroyWindow();
	//-------------------------------------------------------------------------
	void createWindow(HWND parent = NULL);
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
	//-------------------------------------------------------------------------
	static void drawAll();
	//-------------------------------------------------------------------------
protected:
	//-------------------------------------------------------------------------
	void initAsNestedWindow(ArbitraryType::Ptr osParent, const Rectangle &area);
	//-------------------------------------------------------------------------
	WPtr self; // setted during WindowImpl::create()
	//-------------------------------------------------------------------------
	Win32WindowImpl();
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<HWND, Win32WindowImpl*> WinMap;
	//-------------------------------------------------------------------------
	static WinMap winmap;
	//-------------------------------------------------------------------------
	// Impl. by WindowImpl
	virtual void handleMouseButtonPressEvent(int x, int y, int buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void handleMouseButtonReleaseEvent(int x, int y, int buttons) = 0;
	//-------------------------------------------------------------------------
	virtual void handleMouseMotionEvent(int x, int y) = 0;
	//-------------------------------------------------------------------------
	virtual void handleMouseWheelEvent(int x, int y,  int rot) = 0;
	//-------------------------------------------------------------------------
	virtual void onCreated() = 0;
	//-------------------------------------------------------------------------
	virtual void onDestroy() = 0;
	//-------------------------------------------------------------------------
	virtual void boundsUpdated() = 0;
	//-------------------------------------------------------------------------
	virtual void processDraw(sambag::disco::ISurface::Ptr surface) = 0;
public:
	//-------------------------------------------------------------------------
	void invalidateWindow(const Rectangle &area = NULL_RECTANGLE);
	//-------------------------------------------------------------------------
	HWND getHwnd() const {
		return win;
	}
	//-------------------------------------------------------------------------
	void * getSystemHandle() const {
		return getHwnd();
	}
	//-------------------------------------------------------------------------
	static int getNumInstances() {
		return instances;
	}
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
	virtual ~Win32WindowImpl();
	//-------------------------------------------------------------------------
	Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	Rectangle getHostBounds() const;
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &d);
}; // X11Window
}}}

#endif //DISCO_USE_X11
#endif /* SAMBAG_WIN32WINDOW_H */
