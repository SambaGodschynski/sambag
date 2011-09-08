/*
 * CairoDiscoFactory.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef CAIRODISCOFACTORY_HPP_
#define CAIRODISCOFACTORY_HPP_

#include "ADiscoFactory.hpp"

namespace sambag { namespace disco {
//=============================================================================
/**
 * Concrete Cairo Disco Factory.
 */
class CairoDiscoFactory : public ADiscoFactory {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoDiscoFactory> Ptr;
public:
	//-------------------------------------------------------------------------
	// TODO: create context
	//-------------------------------------------------------------------------
	// TODO: create surface
	//-------------------------------------------------------------------------

};
//=============================================================================
}} // namespace


#endif /* CAIRODISCOFACTORY_HPP_ */
