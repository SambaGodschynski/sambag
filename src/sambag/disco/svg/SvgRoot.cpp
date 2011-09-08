/*
 * SvgRoot.hpp.cpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#include "SvgRoot.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
//class SvgRoot
//=============================================================================
//-----------------------------------------------------------------------------
void SvgRoot::init() {
	for_each(SvgObject::Ptr o, svgs) {
		o->init();
	}
}
//-----------------------------------------------------------------------------
void SvgRoot::subObjectCreated( SvgObject::Ptr newObject ) {
	if (newObject.get() == this) { // init self the last element
		init();
		return;
	}
	SvgObject::Ptr self = __self.lock();
	newObject->svgRootObject = self;
	// register id if exists
	if (newObject->getIdName() != "") {
		idMap.insert( std::make_pair("#" + newObject->getIdName(), newObject));
	}
	// register class if exists
	if (newObject->getClassName() != "") {
		classMap.insert( std::make_pair(newObject->getClassName(), newObject));
	}
	svgs.push_back(newObject);
}

}}}
