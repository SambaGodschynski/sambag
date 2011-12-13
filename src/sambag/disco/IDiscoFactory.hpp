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
};
//=============================================================================
//-----------------------------------------------------------------------------
extern IDiscoFactory::Ptr getDiscoFactory();
}} // namespace

#endif /* IDISCOFACTORY_HPP_ */
