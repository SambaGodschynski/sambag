/*
 * ADiscoFactory.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "ADiscoFactory.hpp"
#include "CairoDiscoFactory.hpp"
namespace sambag { namespace disco {

//=============================================================================
// ADiscoFactory
//=============================================================================
//-----------------------------------------------------------------------------
ADiscoFactory::Ptr ADiscoFactory::singleton;
//-----------------------------------------------------------------------------
ADiscoFactory::Ptr ADiscoFactory::getFactory() {
	if (!singleton) {
		singleton = Ptr(new CairoDiscoFactory());
	}
	return singleton;
}
}} // namespace
