/*
 * SVGGraphicElement.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "Svg.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgRoot
//=============================================================================
//-----------------------------------------------------------------------------
void SvgRoot::draw( IDrawContext::Ptr cn ) {
	for_each( SvgObject::Ptr obj, svgObjects ) {
		obj->draw(cn);
	}
}

}}}
