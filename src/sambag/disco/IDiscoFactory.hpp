/*
 * IDiscoFactory.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef IDISCOFACTORY_HPP_
#define IDISCOFACTORY_HPP_

#include "sambag/com/Common.hpp"
#include "sambag/disco/Geometry.hpp"
#include <boost/shared_ptr.hpp>
#include <string>
#include "IDrawContext.hpp"
#include "ISurface.hpp"
#include "IPattern.hpp"

namespace sambag { namespace disco {

//=============================================================================
/**
 * Abstract class DiscoFactory. <Singleton>
 */
class IDiscoFactory {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IDiscoFactory> Ptr;
private:
public:
	//-------------------------------------------------------------------------
	virtual IDrawContext::Ptr createContext( ISurface::Ptr surface ) const = 0;
	//-------------------------------------------------------------------------
	virtual IDrawContext::Ptr createContext() const = 0;
	//-------------------------------------------------------------------------
	virtual IImageSurface::Ptr
	createImageSurface(const Integer &width, const Integer &height) const = 0;
	//-------------------------------------------------------------------------
	virtual IImageSurface::Ptr
	createImageSurface(IDataHandler::Ptr) const = 0;
	//-------------------------------------------------------------------------
	virtual ASolidPattern::Ptr createSolidPattern(const ColorRGBA &col) const = 0;
	//-------------------------------------------------------------------------
	virtual ALinearPattern::Ptr
	createLinearPattern(Point2D p0, Point2D p1) const = 0;
	//-------------------------------------------------------------------------
	virtual ARadialPattern::Ptr
	createRadialPattern(Point2D c0, Number r0,  Point2D c1, Number r1) const = 0;
	//-------------------------------------------------------------------------
	virtual ASurfacePattern::Ptr
	createSurfacePattern(ISurface::Ptr surf) const = 0;
};
//=============================================================================
//-----------------------------------------------------------------------------
extern IDiscoFactory::Ptr getDiscoFactory();
}} // namespace

#endif /* IDISCOFACTORY_HPP_ */
