/*
 * CairoDiscoFactory.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "CairoDiscoFactory.hpp"
#include "CairoDrawContext.hpp"

namespace sambag { namespace disco {
//=============================================================================
// CairoDiscoFactory
//=============================================================================
//-----------------------------------------------------------------------------
IDrawContext::Ptr CairoDiscoFactory::createContext() const {
	CairoSurface::Ptr surf = CairoSurface::create(
		cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL)
	);
	return CairoDrawContext::create(surf);
}

}} // namespace
