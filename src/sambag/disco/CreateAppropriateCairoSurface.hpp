/*
 * CreateAppropriateCairoSurface.hpp
 *
 *  Created on: Fri Jan 25 10:23:00 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CREATEAPPROPRIATECAIROSURFACE_H
#define SAMBAG_CREATEAPPROPRIATECAIROSURFACE_H

#include <cairo.h>
#include "sambag/com/Common.hpp"
namespace sambag { namespace disco {

//-----------------------------------------------------------------------------
/**
 * @return an (os related) appropriate cairo-image surface. 
 */
cairo_surface_t * createAppropriateCairoImageSurface(
	const sambag::com::Integer &width, 
	const sambag::com::Integer &height
);

}} // namespace(s)

#endif /* SAMBAG_CREATEAPPROPRIATECAIROSURFACE_H */
