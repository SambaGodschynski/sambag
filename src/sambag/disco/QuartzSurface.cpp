/*
 * QuartzSurface.hpp
 *
 *  Created on: Jan 8, 2012
 *      Author: samba
 */

//.............................................................................
#ifdef DISCO_USE_COCOA
//.............................................................................
#include "QuartzSurface.hpp"
#include <cairo.h>
//#include <ApplicationServices/ApplicationServices.h> << compiler error
#include <CoreGraphics/CoreGraphics.h>

extern "C"
cairo_public cairo_surface_t *
cairo_quartz_surface_create_for_cg_context (CGContextRef cgContext,
                                            unsigned int width,
                                            unsigned int height);

namespace sambag { namespace disco {
//=============================================================================
// class QuartzSurface
//=============================================================================
//-----------------------------------------------------------------------------
QuartzSurface::Ptr QuartzSurface::create (void *_cg, int width, int height)
{
    /* http://cairographics.org/manual/cairo-Quartz-Surfaces.html
     * If the CGContext is in the Quartz coordinate space
     * (with the origin at the bottom left), then it should be flipped
     * before this function is called.
     */
    CGContextRef cgContext = (CGContextRef)_cg;
    CGContextTranslateCTM (cgContext, 0.0, height);
    CGContextScaleCTM (cgContext, 1.0, -1.0);
            
    cairo_surface_t *s = cairo_quartz_surface_create_for_cg_context(
        cgContext, width, height);
    Ptr neu = Ptr(new QuartzSurface(s));
    neu->_size = Rectangle(0,0,width,height);
    return neu;
}
//-----------------------------------------------------------------------------
Rectangle QuartzSurface::getSize() const {
    return _size;
}
//-----------------------------------------------------------------------------
Rectangle QuartzSurface::getClipRect() const {
    return _size;
}
//-----------------------------------------------------------------------------
QuartzSurface::~QuartzSurface() {
}
}}
#endif //DISCO_USE_COCOA


