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
void Compound::draw( IDrawContext::Ptr cn ) {
	for_each( GraphicElement::Ptr obj, objects ) {
		obj->draw(cn);
	}
}
//-------------------------------------------------------------------------
void Compound::copyStyleFrom( const Style &b ) {
	GraphicElement::copyStyleFrom(b);
	for_each( GraphicElement::Ptr obj, objects ) {
		obj->copyStyleFrom(b);
	}
}
//-------------------------------------------------------------------------
void Compound::setStyle( const Style &_style ) {
	GraphicElement::setStyle(_style);
	for_each( GraphicElement::Ptr obj, objects ) {
		obj->setStyle(_style);
	}
}

}}}

