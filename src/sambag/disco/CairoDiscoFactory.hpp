/*
 * CairoDiscoFactory.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef CAIRODISCOFACTORY_HPP_
#define CAIRODISCOFACTORY_HPP_

#include "IDiscoFactory.hpp"

namespace sambag { namespace disco {
//=============================================================================
/**
 * Concrete Cairo Disco Factory.
 */
class CairoDiscoFactory : public IDiscoFactory {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoDiscoFactory> Ptr;
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

};
//=============================================================================
}} // namespace


#endif /* CAIRODISCOFACTORY_HPP_ */
