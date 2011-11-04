/*
 * PngSurface.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef PNGSURFACE_HPP_
#define PNGSURFACE_HPP_

#include "cairo.h"
#include "ISurface.hpp"

namespace sambag { namespace disco {
//=============================================================================
class CairoSurface : public ISurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoSurface> Ptr;
protected:
	//-------------------------------------------------------------------------
	cairo_surface_t * surface;
	//-------------------------------------------------------------------------
	Rectangle & getImageSurfaceRect(Rectangle &res) const {
		res.setWidth((Number)cairo_image_surface_get_width(surface));
		res.setHeight((Number)cairo_image_surface_get_height(surface));
		return res;
	}
	//-------------------------------------------------------------------------
	Rectangle & getRecordingSurfaceRect(Rectangle &res) const {
		Number x, y, w, h;
		cairo_recording_surface_ink_extents (surface, &x, &y, &w, &h);
		res = Rectangle(Point2D(x,y), w, h);
		return res;
	}
	//-------------------------------------------------------------------------
	CairoSurface(cairo_surface_t *s) : surface(s) {}
public:
	//-------------------------------------------------------------------------
	cairo_surface_t * getCairoSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	static Ptr create(cairo_surface_t *s) {
		return Ptr (new CairoSurface(s));
	}
	//-------------------------------------------------------------------------
	virtual ~CairoSurface() {
		if (!surface)
			return;
		cairo_surface_destroy(surface);
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const {
		Rectangle res(0,0,0,0);
		if (!surface)
			return res;
		switch (cairo_surface_get_type(surface)) {
			case CAIRO_SURFACE_TYPE_IMAGE :
				return getImageSurfaceRect(res);
			case CAIRO_SURFACE_TYPE_RECORDING :
				return getRecordingSurfaceRect(res);
			case CAIRO_SURFACE_TYPE_QUARTZ: // TODO
			case CAIRO_SURFACE_TYPE_WIN32:  // TODO
			default:
				break;
		}
		return res;
	}
	//-------------------------------------------------------------------------
};

}} // namespace

#endif /* PNGSURFACE_HPP_ */
