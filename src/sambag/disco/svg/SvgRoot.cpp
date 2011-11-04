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
	for_each(SvgObject::Ptr o, svgs) { // TODO: remove?
		o->init();
	}
}
//-----------------------------------------------------------------------------
void SvgRoot::subObjectCreated( SvgObject::Ptr newObject ) {
	if (newObject.get() == this) { // init self
		init();
		return;
	}
	SvgObject::Ptr self = __self.lock();
	newObject->svgRootObject = self;
	svgs.push_back(newObject);
}

}}}
