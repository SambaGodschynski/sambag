/*
 * CompoundElement.cpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#include "SvgCompound.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace svg {

// TODO: make base class for compound objects

//=============================================================================
// class SvgCompound
//=============================================================================
//-----------------------------------------------------------------------------
void SvgCompound::draw( IDrawContext::Ptr cn ) {
	if (!combo) return;
	AutoTransform at(getTransformMatrix(), cn);
	combo->draw(cn);
}

}}}

