/*
 * ADiscoFactory.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "IDiscoFactory.hpp"
#include "CairoDiscoFactory.hpp"
namespace sambag { namespace disco {
//-----------------------------------------------------------------------------
IDiscoFactory::Ptr singleton;
//-----------------------------------------------------------------------------
IDiscoFactory::Ptr getDiscoFactory() {
	if (!singleton) {
		singleton = IDiscoFactory::Ptr(new CairoDiscoFactory());
	}
	return singleton;
}
}} // namespace
