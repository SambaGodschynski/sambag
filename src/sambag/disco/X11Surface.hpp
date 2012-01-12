/*
 * X11Surface.hpp
 *
 *  Created on: Jan 8, 2012
 *      Author: samba
 */

#ifndef X11SURFACE_HPP_
#define X11SURFACE_HPP_

//.............................................................................
#ifdef DISCO_USE_X11
//.............................................................................

#include <X11/Xlib.h>
#include <cairo-xlib.h>
#include "CairoSurface.hpp"


namespace sambag { namespace disco {
//=============================================================================
class X11Surface : public CairoSurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<X11Surface> Ptr;
protected:
	//-------------------------------------------------------------------------
	X11Surface(cairo_surface_t *s) : CairoSurface(s) {}
public:
	//-------------------------------------------------------------------------
	Ptr static create (Display *dpy,
			Drawable drawable,
			Visual *visual,
			int width,
			int height)
	{
		cairo_surface_t *s = cairo_xlib_surface_create(dpy,
				drawable,
				visual,
				width,
				height);
		Ptr neu = Ptr(new X11Surface(s));
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const {
		return Rectangle ( Point2D(),
			cairo_xlib_surface_get_width(surface),
			cairo_xlib_surface_get_height(surface)
		);
	}
	//-------------------------------------------------------------------------
	virtual void setSize(const Rectangle &r) {
		cairo_xlib_surface_set_size(surface,
			r.getWidth(),
			r.getHeight()
		);
	}
};
}}
//.............................................................................
#endif // DISCO_USE_X11
//.............................................................................
#endif /* X11SURFACE_HPP_ */
