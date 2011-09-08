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
	// TODO: create context
	//-------------------------------------------------------------------------
	// TODO: create surface
	//-------------------------------------------------------------------------
};
//=============================================================================
}} // namespace

#endif /* IDISCOFACTORY_HPP_ */
