/*
 * StyleAttributeSetters.cpp
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
	style.opacity(v);
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
	std::stringstream is;
	is<<value;
	int v;
	is>>v;
	if (v<=Font::SLANT_NUM_MIN || v >= Font::NUM_SLANT)
		v = Font::SLANT_UNDEFINED;
	style.fontSlant(static_cast<Font::Slant>(v));
}
//-----------------------------------------------------------------------------
void setFontWeight( Style &style, const std::string &value ) {
	std::stringstream is;
	is<<value;
	int v;
	is>>v;
	if (v<=Font::WEIGHT_NUM_MIN || v >= Font::NUM_WEIGHT)
		v = Font::WEIGHT_UNDEFINED;
	style.fontWeight(static_cast<Font::Weight>(v));
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
	("font-size", &setFontSize)
	("font-family", &setFontFace)
	("font-weight", &setFontWeight)
	("font-style", &setFontSlant)
	("fill", &setFillColor);
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
