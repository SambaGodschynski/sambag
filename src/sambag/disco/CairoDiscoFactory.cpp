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
	CairoSurface::Ptr surf = CairoRecordingSurface::create();
	return createContext(surf);
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr CairoDiscoFactory::createContext(ISurface::Ptr surface) const {
	CairoSurface::Ptr surf = boost::shared_dynamic_cast<CairoSurface>(surface);
	if (!surf)
		return CairoDrawContext::Ptr();
	return CairoDrawContext::create(surf);
}
//-----------------------------------------------------------------------------
IImageSurface::Ptr CairoDiscoFactory::createImageSurface(const Integer &width,
		const Integer &height) const
{
	return CairoImageSurface::create(width, height);
}
//-----------------------------------------------------------------------------
IImageSurface::Ptr CairoDiscoFactory::createImageSurface(IDataHandler::Ptr handler) const {
	return CairoImageSurface::create(handler);
}

}} // namespace
