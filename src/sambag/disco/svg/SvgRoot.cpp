/*
 * SvgRoot.hpp.cpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#include "SvgRoot.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
//class SvgRoot
//=============================================================================
//-----------------------------------------------------------------------------
void SvgRoot::subObjectCreated( SvgObject::Ptr newObject ) {
	SvgObject::Ptr self =
			boost::shared_dynamic_cast<SvgObject, GraphicElement>(__self.lock());
	newObject->svgRootObject = self;
	// register id if exists
	if (newObject->getId() != NULL_ID) {
		idMap.insert( std::make_pair(newObject->getId(), newObject));
	}
	// register class if exists
	if (newObject->getClass() != NULL_CLASS) {
		classMap.insert( std::make_pair(newObject->getClass(), newObject));
	}
}

}}}
