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
		res.setWidth((Number)cairo_image_surface_get_width(surface));
		res.setHeight((Number)cairo_image_surface_get_height(surface));
		return res;
	}
	//-------------------------------------------------------------------------
};

}} // namespace

#endif /* PNGSURFACE_HPP_ */
