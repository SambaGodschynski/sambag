/*
 * CairoDiscoFactory.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef CAIRODISCOFACTORY_HPP_
#define CAIRODISCOFACTORY_HPP_

#include "IDiscoFactory.hpp"
#include <loki/Singleton.h>

namespace sambag { namespace disco {
//=============================================================================
/**
 * Concrete Cairo Disco Factory.
 */
class CairoDiscoFactory : public IDiscoFactory {
//=============================================================================
friend struct Loki::CreateUsingNew<CairoDiscoFactory>;
private:
	CairoDiscoFactory() {}
	CairoDiscoFactory(const CairoDiscoFactory&) {}
public:
	//-------------------------------------------------------------------------
	virtual IDrawContext::Ptr createContext( ISurface::Ptr surface ) const;
	//-------------------------------------------------------------------------
	/**
	 * creates context with ciaro record surface
	 * see: http://cairographics.org/manual/cairo-Recording-Surfaces.html
	 * @return
	 */
	virtual IDrawContext::Ptr createContext() const;
	//-------------------------------------------------------------------------
	virtual IImageSurface::Ptr
	createImageSurface(const Integer &width, const Integer &height) const;
	//-------------------------------------------------------------------------
	virtual IImageSurface::Ptr
	createImageSurface(IDataHandler::Ptr handler) const;
	//-------------------------------------------------------------------------
	virtual IRecordingSurface::Ptr
	createRecordingSurface() const;
	//-------------------------------------------------------------------------
	virtual IRecordingSurface::Ptr
	createRecordingSurface(const Integer &width, const Integer &height) const;
	//-------------------------------------------------------------------------
	ISolidPattern::Ptr createSolidPattern(const ColorRGBA &col) const;
	//-------------------------------------------------------------------------
	ILinearPattern::Ptr
	createLinearPattern(Point2D p0, Point2D p1) const;
	//-------------------------------------------------------------------------
	IRadialPattern::Ptr
	createRadialPattern(Point2D c0, Number r0,  Point2D c1, Number r1) const;
	//-------------------------------------------------------------------------
	ISurfacePattern::Ptr
	createSurfacePattern(ISurface::Ptr surf) const;

};
//=============================================================================
}} // namespace


#endif /* CAIRODISCOFACTORY_HPP_ */
