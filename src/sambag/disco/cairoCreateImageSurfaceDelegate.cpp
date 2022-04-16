/**
 * File: cairoCreateImageSurface.cpp
 * Date: 2.7.2013
 * Author: sambag
 */
#include <cairo.h>
extern "C" cairo_surface_t * cairo_quartz_surface_create (cairo_format_t format,
                                                          unsigned int width,
                                                          unsigned int height);

//-----------------------------------------------------------------------------
cairo_surface_t * disco_cairo_createImageSurface(int width, int height) {
#ifdef DISCO_USE_COCOA
    return cairo_quartz_surface_create(CAIRO_FORMAT_ARGB32, width, height);
#else
    return cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
#endif
}