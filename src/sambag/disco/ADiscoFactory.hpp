/*
 * IDiscoFactory.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef IDISCOFACTORY_HPP_
#define IDISCOFACTORY_HPP_

#include "sambag/com/Common.hpp"
#include <boost/shared_ptr.hpp>
#include <string>
#include "IDrawContext.hpp"
#include "ISurface.hpp"

namespace sambag { namespace disco {

//=============================================================================
/**
 * Abstract class DiscoFactory. <Singleton>
 */
class ADiscoFactory {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ADiscoFactory> Ptr;
private:
	//-------------------------------------------------------------------------
	static Ptr singleton;
public:
	//-------------------------------------------------------------------------
	static Ptr getFactory();
	//-------------------------------------------------------------------------
	virtual IDrawContext::Ptr createContext( ISurface::Ptr surface ) const = 0;
	//-------------------------------------------------------------------------
	virtual IDrawContext::Ptr createContext() const = 0;
	//-------------------------------------------------------------------------
	// TODO: create surface
	//-------------------------------------------------------------------------
};
//=============================================================================
}} // namespace

#endif /* IDISCOFACTORY_HPP_ */
