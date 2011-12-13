/*
 * CairoPattern.cpp
 *
 *  Created on: Dec 15, 2011
 *      Author: samba
 */

#include "CairoPattern.hpp"
#include "CairoSurface.hpp"

namespace sambag { namespace disco {
//=============================================================================
// class CairoSurfacePattern
//=============================================================================
//-----------------------------------------------------------------------------
CairoSurfacePattern::Ptr CairoSurfacePattern::create(ISurface::Ptr _surf) {
	CairoSurface::Ptr surf =
		boost::shared_dynamic_cast<CairoSurface>(_surf);
	if (!surf)
		return Ptr();
	CairoPatternRef ref = createPatternRef(
		cairo_pattern_create_for_surface(surf->getCairoSurface())
	);
	Ptr neu(new CairoSurfacePattern(ref));
	neu->surface = surf;
	return neu;
}
}}
