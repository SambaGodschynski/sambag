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
void SvgRoot::initCreatedObjects() {
	boost_for_each(SvgObject::Ptr o, svgs) {
		o->init();
	}
}
//-----------------------------------------------------------------------------
void SvgRoot::subObjectCreated( SvgObject::Ptr newObject,
	const std::string &tagName )
{
	newObject->setTagName(tagName);
	if (newObject.get() == this) { // don't add self
		return;
	}
	SvgObject::Ptr self = __self.lock();
	newObject->svgRootObject = self;
	svgs.push_back(newObject);
}

}}}
