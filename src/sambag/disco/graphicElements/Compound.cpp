/*
 * CompoundElement.cpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#include "Compound.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
// class Compound
//=============================================================================
//-----------------------------------------------------------------------------
Rectangle Compound::getBoundingBox() const {
	Rectangle res;
	return NULL_RECTANGLE;
}
//-----------------------------------------------------------------------------
GraphicElement::Ptr Compound::clone() const {
	Ptr neu = create();
	*neu = *this;

//	TODO: copy scenegraph! neu->objects.clear();
//	for_each( GraphicElement::Ptr obj, objects ) {
//		neu->add(obj->clone());
//	}
	return neu;
}
//-----------------------------------------------------------------------------
void Compound::draw( IDrawContext::Ptr cn ) {
}
}}}

