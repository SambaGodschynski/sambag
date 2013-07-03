/**
 * File: cairoCGContextWorkaround.cpp
 * Date: 2.7.2013
 * Author: sambag
 * 
 * problems with including <ApplicationServices/ApplicationServices.h> 
 * (produces a bunch of compiler errors)
 * are the reason for this file.
 */
#include <cairo.h>
#include <cairo-quartz.h>

//-----------------------------------------------------------------------------
extern
cairo_surface_t *
disco_cairo_quartz_surface_create (void *_cg,
                                   unsigned int width,
                                   unsigned int height)
{
    CGContextRef cgContext = (CGContextRef)_cg;
    CGContextTranslateCTM (cgContext, 0.0, height);
    CGContextScaleCTM (cgContext, 1.0, -1.0);
    
    cairo_surface_t *s = cairo_quartz_surface_create_for_cg_context(
        cgContext, width, height
    );
    return s;
}

