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
//-----------------------------------------------------------------------------
SvgObject::IdType SvgObject::NULL_ID = "no-id";
//-----------------------------------------------------------------------------
SvgObject::ClassType SvgObject::NULL_CLASS = "no-class";
//-----------------------------------------------------------------------------
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
	binder.registerAttribute
		<Id_tag::Type, Id_tag, SvgObject>
			("id");
	binder.registerAttribute
		<Class_tag::Type, Class_tag, SvgObject>
			("class");
	binder.registerAttribute
		<Transform_tag::Type, Transform_tag, SvgObject>
			("transform");
	// TODO: register more style tags
}

}}} // namespaces
