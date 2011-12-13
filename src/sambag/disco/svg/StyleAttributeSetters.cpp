/*
 * StyleAttributeSetters.cpp
 * Another file for StyleParser.
 *
 *  Created on: 14.10.2011
 *      Author: samba
 */

#include "StyleParser.hpp"
#include "AttributeParser.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_inserter.hpp>
#include "sambag/com/Common.hpp"
#include <sstream>
namespace { // setter
using namespace sambag::disco;
using namespace sambag::com;
using sambag::disco::graphicElements::Style;
//-----------------------------------------------------------------------------
void setStrokeColor( Style &style, const std::string &value ) {
	std::stringstream is;
	is<<value;
	ColorRGBA col;
	is>>col;
	style.strokeColor(col);
}
//-----------------------------------------------------------------------------
void setStrokeWitdh( Style &style, const std::string &value ) {
	std::stringstream is;
	is<<value;
	Number v;
	is>>v;
	style.strokeWidth(v);
}
//-----------------------------------------------------------------------------
void setFillColor( Style &style, const std::string &value ) {
	std::stringstream is;
	is<<value;
	ColorRGBA col;
	is>>col;
	style.fillColor(col);
}
//-----------------------------------------------------------------------------
void setOpacity( Style &style, const std::string &value ) {
	Number v=1.0;
	sambag::disco::svg::AttributeParser::parseOpacity(value, v);
	style.strokeOpacity(v);
	style.fillOpacity(v);
}
//-----------------------------------------------------------------------------
void setStrokeOpacity( Style &style, const std::string &value ) {
	Number v=1.0;
	sambag::disco::svg::AttributeParser::parseOpacity(value, v);
	style.strokeOpacity(v);
}
//-----------------------------------------------------------------------------
void setFillOpacity( Style &style, const std::string &value ) {
	Number v=1.0;
	sambag::disco::svg::AttributeParser::parseOpacity(value, v);
	style.fillOpacity(v);
}
//-----------------------------------------------------------------------------
void setStrokeDashArray( Style &style, const std::string &value ) {
	Dash::Ptr dash = svg::AttributeParser::parseDashArray(value);
	if (style.dash()) {
		dash->offset(style.dash()->offset());
	}
	style.dash(dash);
}
//-----------------------------------------------------------------------------
void setStrokeDashOffset( Style &style, const std::string &value ) {
	if (!style.dash()) {
		style.dash(Dash::create(0));
	}
	Coordinate c;
	svg::AttributeParser::parseCoordinate(value, c);
	style.dash()->offset(c);
}
//-----------------------------------------------------------------------------
void setFontSize( Style &style, const std::string &value ) {
	std::stringstream is;
	is<<value;
	Number v;
	is>>v;
	style.fontSize(v);
}
//-----------------------------------------------------------------------------
void setFontFace( Style &style, const std::string &value ) {
	style.fontFace(value);
}
//-----------------------------------------------------------------------------
void setFontSlant( Style &style, const std::string &value ) {
	std::string in = value;
	svg::AttributeParser::prepareString(in);
	Font::Slant v = Font::SLANT_UNDEFINED;
	if (in=="normal")
		v = Font::SLANT_NORMAL;
	if (in=="italic")
			v = Font::SLANT_ITALIC;
	if (in=="oblique")
			v = Font::SLANT_OBLIQUE;
	style.fontSlant(v);
}
//-----------------------------------------------------------------------------
void setFontWeight( Style &style, const std::string &value ) {
	std::string in = value;
	svg::AttributeParser::prepareString(in);
	Font::Weight v = Font::WEIGHT_UNDEFINED;
	if (in=="normal")
		v = Font::WEIGHT_NORMAL;
	if (in=="bold")
		v = Font::WEIGHT_BOLD;
	style.fontWeight(v);
}
//-----------------------------------------------------------------------------
void setLineCapStyle( Style &style, const std::string &value ) {
	std::stringstream ss(value);
	IDrawContext::LineCapStyle v = IDrawContext::NO_LINE_CAP;
	ss>>v;
	style.lineCapStyle(v);
}
//-----------------------------------------------------------------------------
void setLineJoinStyle( Style &style, const std::string &value ) {
	std::stringstream ss(value);
	IDrawContext::LineJoin v = IDrawContext::NO_LINE_JOIN;
	ss>>v;
	style.lineJoin(v);
}
//-----------------------------------------------------------------------------
void setMiterLimit( Style &style, const std::string &value ) {
	std::stringstream ss(value);
	Number v = 0.;
	ss>>v;
	style.miterLimit(v);
}


}//namespace


namespace sambag { namespace disco { namespace svg {
//=============================================================================
// Parser
//=============================================================================
//-----------------------------------------------------------------------------
StyleParser::SetterMap StyleParser::attr2Setter;
//-----------------------------------------------------------------------------
void StyleParser::initSetterMap() {
	using namespace boost::assign;
	insert(attr2Setter)
	("stroke", &setStrokeColor)
	("stroke-width", &setStrokeWitdh)
	("opacity", &setOpacity)
	("stroke-opacity", &setStrokeOpacity)
	("stroke-dasharray", &setStrokeDashArray)
	("stroke-dashoffset", &setStrokeDashOffset)
	("fill-opacity", &setFillOpacity)
	("font-size", &setFontSize)
	("font-family", &setFontFace)
	("font-weight", &setFontWeight)
	("font-style", &setFontSlant)
	("fill", &setFillColor)
	("stroke-linecap", &setLineCapStyle)
	("stroke-linejoin", &setLineJoinStyle)
	("stroke-miterlimit", &setMiterLimit);
}
//-----------------------------------------------------------------------------
StyleParser::SetStyleAttributeFunc StyleParser::getSetter(const std::string& str) {
	using boost::algorithm::to_lower_copy;
	if (attr2Setter.empty())
		initSetterMap();
	std::string key = to_lower_copy(str);
	SetterMap::iterator it = attr2Setter.find(key);
	if (it==attr2Setter.end())
		return NULL;
	return it->second;
}
}}} // namespaces
