/*
 * CreateAppropriateCairoSurface.cpp
 *
 *  Created on: Fri Jan 25 10:23:00 2013
 *      Author: Johannes Unger
 */

#include "CreateAppropriateCairoSurface.hpp"

namespace sambag { namespace disco {
using sambag::com::Integer;
namespace {
//.............................................................................
#ifdef DISCO_USE_WIN32_DIABLE_
//.............................................................................
#include <cairo-win32.h>
cairo_surface_t * _createImageSf(const Integer &width, 
	const Integer &height) 
{
		cairo_surface_t *s = cairo_win32_surface_create_with_dib(
			CAIRO_FORMAT_ARGB32, width, height
		);
		return s;
}
//.............................................................................
#else
//.............................................................................
cairo_surface_t * _createImageSf(const Integer &width, 
	const Integer &height) 
{
		cairo_surface_t *s = cairo_image_surface_create(
			CAIRO_FORMAT_ARGB32, width, height
		);
		return s;
}
#endif
} // namespace(s)

//=============================================================================
//  CreateAppropriateCairoSurface
//=============================================================================
//-----------------------------------------------------------------------------
cairo_surface_t * createAppropriateCairoImageSurface(const Integer &width, 
	const Integer &height) 
{
	return _createImageSf(width, height);
}

}} // namespace(s)
