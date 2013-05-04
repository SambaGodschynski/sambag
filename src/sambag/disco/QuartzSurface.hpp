/*
 * QuartzSurface.hpp
 *
 *  Created on: Jan 8, 2012
 *      Author: samba
 */

#ifndef QUARTZSURFACE_HPP_
#define QUARTZSURFACE_HPP_

//.............................................................................
#ifdef DISCO_USE_COCOA
//.............................................................................
#include <cairo-quartz.h>
#include "CairoSurface.hpp"

namespace sambag { namespace disco {
//=============================================================================
class QuartzSurface : public CairoSurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<QuartzSurface> Ptr;
protected:
	//-------------------------------------------------------------------------
	QuartzSurface(cairo_surface_t *s) : CairoSurface(s) {}
	//-------------------------------------------------------------------------
	sambag::disco::Rectangle _size;
public:
	//-------------------------------------------------------------------------
	Ptr static create (CGContextRef cgContext, int width, int height) {
		/* http://cairographics.org/manual/cairo-Quartz-Surfaces.html
		 * If the CGContext is in the Quartz coordinate space
		 * (with the origin at the bottom left), then it should be flipped
		 * before this function is called.
		 */
		CGContextTranslateCTM (cgContext, 0.0, height);
		CGContextScaleCTM (cgContext, 1.0, -1.0);

		cairo_surface_t *s = cairo_quartz_surface_create_for_cg_context(cgContext, width, height);
		Ptr neu = Ptr(new QuartzSurface(s));
		neu->_size = Rectangle(0,0,width,height);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const {
		return _size;
	}
	//-------------------------------------------------------------------------
	/**
	 * default impl. 
	 * @return getSize();
	 */
	virtual Rectangle getClipRect() const {
		return _size;
	}
	//-------------------------------------------------------------------------
	virtual void setSize(const Rectangle &r) {
	}
};
}}
//.............................................................................
#endif //DISCO_USE_COCOA
//.............................................................................
#endif /* WIN32SURFACE_HPP_ */
