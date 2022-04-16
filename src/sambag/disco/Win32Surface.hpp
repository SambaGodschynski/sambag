/*
 * X11Surface.hpp
 *
 *  Created on: Jan 8, 2012
 *      Author: samba
 */

#ifndef WIN32SURFACE_HPP_
#define WIN32SURFACE_HPP_

//.............................................................................
#ifdef DISCO_USE_WIN32
//.............................................................................

#include "CairoSurface.hpp"
#include <cairo-win32.h>
#include <windows.h>

namespace sambag { namespace disco {
//=============================================================================
class Win32Surface : public CairoSurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Win32Surface> Ptr;
protected:
	//-------------------------------------------------------------------------
	Win32Surface(cairo_surface_t *s) : CairoSurface(s) {}
	//-------------------------------------------------------------------------
	Rectangle clipRect;
public:
	//-------------------------------------------------------------------------
	Ptr static create (HDC dc) {
		cairo_surface_t *s = cairo_win32_surface_create(dc);
		Ptr neu = Ptr(new Win32Surface(s));
		return neu;
	}
	//-------------------------------------------------------------------------
	Ptr static create (HDC dc, int width, int height) {
		cairo_surface_t *s = cairo_win32_surface_create_with_ddb(
			dc,
			CAIRO_FORMAT_RGB24,
			width,
			height
		);
		Ptr neu = Ptr(new Win32Surface(s));
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const {
		RECT r;
		HWND hWnd = WindowFromDC (
			cairo_win32_surface_get_dc( getCairoSurface() )
		);
		if (!hWnd) {
			return NULL_RECTANGLE;
		}
		GetClientRect(hWnd, &r);
		Rectangle res( Point2D(r.left, r.top),
			Point2D(r.right, r.bottom)
		);
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * default impl. 
	 * @return getSize();
	 */
	virtual Rectangle getClipRect() const {
		RECT r;
		HDC hdc = cairo_win32_surface_get_dc( getCairoSurface() );
		if (!hdc) {
			return NULL_RECTANGLE;
		}
		if ( GetClipBox(hdc, &r) == ERROR ) {
			return NULL_RECTANGLE;
		}
		Rectangle res( Point2D(r.left, r.top),
			Point2D(r.right, r.bottom)
		);
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void setSize(const Rectangle &r) {
	}
};
}}
//.............................................................................
#endif // DISCO_USE_WIN32
//.............................................................................
#endif /* WIN32SURFACE_HPP_ */
