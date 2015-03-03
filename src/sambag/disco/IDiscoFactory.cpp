/*
 * ADiscoFactory.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "IDiscoFactory.hpp"
#include "CairoDiscoFactory.hpp"
#include <loki/Singleton.h>
namespace sambag { namespace disco {
namespace {
	typedef Loki::SingletonHolder<CairoDiscoFactory> DiscoFactoryHolder;
}
//-----------------------------------------------------------------------------
IDiscoFactory * getDiscoFactory() {
	return &DiscoFactoryHolder::Instance();
}
}} // namespace
