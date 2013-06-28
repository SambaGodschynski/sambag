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
	Ptr static create (void *cgContext, int width, int height);
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const;
	//-------------------------------------------------------------------------
	/**
	 * default impl. 
	 * @return getSize();
	 */
	virtual Rectangle getClipRect() const;
	//-------------------------------------------------------------------------
	virtual void setSize(const Rectangle &r) {
	}
	//-------------------------------------------------------------------------
	virtual ~QuartzSurface();
};
}}
//.............................................................................
#endif //DISCO_USE_COCOA
//.............................................................................
#endif /* QUARTZSURFACE_HPP_ */
