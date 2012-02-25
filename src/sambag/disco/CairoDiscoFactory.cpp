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
IImageSurface::Ptr
CairoDiscoFactory::createImageSurface(IDataHandler::Ptr handler) const {
	return CairoImageSurface::create(handler);
}
//-------------------------------------------------------------------------
ISolidPattern::Ptr
CairoDiscoFactory::createSolidPattern(const ColorRGBA &col) const {
	return CairoSolidPattern::create(col);
}
//-------------------------------------------------------------------------
ILinearPattern::Ptr
CairoDiscoFactory::
createLinearPattern(Point2D p0, Point2D p1) const
{
	return CairoLinearPattern::create(
		ILinearPattern::LinearPoints(p0, p1)
	);
}
//-------------------------------------------------------------------------
IRadialPattern::Ptr
CairoDiscoFactory::
createRadialPattern(Point2D c0, Number r0,  Point2D c1, Number r1) const
{
	return CairoRadialPattern::create(
		IRadialPattern::RadialCircles(c0, r0, c1, r1)
	);
}
//-------------------------------------------------------------------------
ISurfacePattern::Ptr
CairoDiscoFactory::createSurfacePattern(ISurface::Ptr surf) const {
	return CairoSurfacePattern::create(surf);
}

}} // namespace
