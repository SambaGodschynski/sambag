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
// class SvgObject
//=============================================================================
void SvgObject::registerAttributes( SvgObject::BuilderType &binder ) {
	binder.registerAttribute
		<StrokeWidth_tag::Type, StrokeWidth_tag, SvgObject>
			("stroke-width");
	binder.registerAttribute
		<Stroke_tag::Type, Stroke_tag, SvgObject>
			("stroke");
	binder.registerAttribute
		<Fill_tag::Type, Fill_tag, SvgObject>
			("fill");
	// TODO: register more style tags
}

}}} // namespaces
