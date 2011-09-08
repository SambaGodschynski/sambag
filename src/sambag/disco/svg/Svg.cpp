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
	binder.registerAttribute
		<OpacityStyle_tag::Type, OpacityStyle_tag, SvgObject>
			("opacity");
	binder.registerAttribute
		<Style_tag::Type, Style_tag, SvgObject>
			("style");
	binder.registerAttribute<FontFamily_tag::Type, FontFamily_tag, SvgObject>("font-family");
	binder.registerAttribute<FontSize_tag::Type, FontSize_tag, SvgObject>("font-size");
	binder.registerAttribute<FontWeight_tag::Type, FontWeight_tag, SvgObject>("font-weight");
	binder.registerAttribute<FontStyle_tag::Type, FontStyle_tag, SvgObject>("font-style");
	// TODO: register more style tags
}

}}} // namespaces
