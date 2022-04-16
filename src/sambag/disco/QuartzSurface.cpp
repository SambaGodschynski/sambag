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
//#include <CoreGraphics/CoreGraphics.h>

extern
cairo_surface_t *
disco_cairo_quartz_surface_create (void *cgContext,
                             unsigned int width,
                             unsigned int height);

namespace sambag { namespace disco {
//=============================================================================
// class QuartzSurface
//=============================================================================
//-----------------------------------------------------------------------------
QuartzSurface::Ptr QuartzSurface::create (void *cg, int width, int height)
{
    /* http://cairographics.org/manual/cairo-Quartz-Surfaces.html
     * If the CGContext is in the Quartz coordinate space
     * (with the origin at the bottom left), then it should be flipped
     * before this function is called.
     */
    cairo_surface_t *s = disco_cairo_quartz_surface_create(
        cg, width, height);
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


