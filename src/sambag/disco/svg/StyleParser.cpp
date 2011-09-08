/*
 * StyleParser.cpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#include "StyleParser.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_inserter.hpp>

namespace {
	int hex2Int ( const std::string &hex) {
		int x = 0;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> x;
		return x;
	}
}

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// Parser
//=============================================================================
//-----------------------------------------------------------------------------
StyleParser::HtmlColors StyleParser::htmlColors;
//-----------------------------------------------------------------------------
void StyleParser::parseColor(const std::string &str, ColorRGBA &color) {
	static const boost::regex colorname("[a-zA-Z]+");
	static const boost::regex colorval("#[0-9a-fA-F]+");
	if ( boost::regex_match(str, colorname) ) { // is colorname
		const ColorRGBA &c = getColorByHtmlName(str);
		color.setValues(c.getR(), c.getG(), c.getB(), c.getA());
		return;
	}
	if ( !boost::regex_match(str, colorval) ) return;
	// is color html value
	std::string expr = boost::algorithm::trim_copy(str);
	size_t num = expr.length();
	while (num++<7) { // append missing zeros
		expr.append("0");
	}
	Number r = hex2Int(expr.substr(1,2)) / 255.0;
	Number g = hex2Int(expr.substr(3,2)) / 255.0;
	Number b = hex2Int(expr.substr(5,2)) / 255.0;
	Number a = 255;

	color.setValues(r,g,b,a);
}
//-----------------------------------------------------------------------------
const ColorRGBA & StyleParser::getColorByHtmlName( const std::string &name ) {
	using boost::algorithm::to_lower_copy;
	if (htmlColors.empty())
		initHtmlColors();
	std::string key = to_lower_copy(name);
	HtmlColors::const_iterator it = htmlColors.find(key);
	if (it==htmlColors.end())
		return NULL_COLOR;
	return it->second;
}
//-----------------------------------------------------------------------------
std::string & StyleParser::prepareString(std::string &inout) {
	boost::algorithm::trim(inout);
	boost::algorithm::to_lower(inout);
	return inout;
}
}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::com::ColorRGBA& color) {
	using namespace sambag::disco::svg;
	std::string str;
	istr>>str;
	StyleParser::parseColor(str, color);
	return istr;
}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::Font::Weight &weight) {
	using namespace sambag::disco;
	std::string in;
	istr>>in;
	svg::StyleParser::prepareString(in);
	if (in=="bold") weight = Font::WEIGHT_BOLD;
	else weight = Font::WEIGHT_NORMAL;
	return istr;

}
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::Font::Slant &slant) {
	using namespace sambag::disco;
	std::string in;
	istr>>in;
	svg::StyleParser::prepareString(in);
	if (in=="italic") slant = Font::SLANT_ITALIC; return istr;
	if (in=="oblique") slant = Font::SLANT_OBLIQUE; return istr;
	slant = Font::SLANT_NORMAL;
	return istr;
}
