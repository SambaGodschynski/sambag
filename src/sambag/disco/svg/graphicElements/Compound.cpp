/*
 * CompoundElement.cpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#include "Compound.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {

//=============================================================================
// class Compound
//=============================================================================
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
}}}}

