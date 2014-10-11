/*
 * StyleParser.cpp
 *
 *  Created on: 14.10.2011
 *      Author: samba
 */

#include "StyleParser.hpp"

#include "AttributeParser.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/assign/list_inserter.hpp>
#include <vector>
#include <sstream>
#include <math.h>


namespace sambag { namespace disco { namespace svg {
//=============================================================================
// StyleParser
//=============================================================================
//-----------------------------------------------------------------------------
sambag::disco::svg::Style createStyle(const std::string &str) {
	using namespace io;
	sambag::disco::svg::Style res;
	res.font(sambag::disco::Font()); // TODO: why has sytle no std font?
	std::stringstream ss;
	ss<<str;
	ss>>res;
	return res;
}
}}} // namespaces
namespace {
using namespace sambag::disco;
//-----------------------------------------------------------------------------
/**
 * parses style properties eg.: "stroke: red; stroke-width:2px;"
 * @param properties
 * @param outStyle
 */
void parseStyleProperties( const std::string &properties,
		svg::Style& outStyle)
{
	using namespace sambag::disco::svg;
	if (properties.length()==0) return;
	std::string inStr = properties;
	AttributeParser::prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("([a-zA-Z-]+)\\s*:\\s*([a-zA-Z0-9#%.\"':/ ]+)\\s*;{0,1}");
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
		std::string attribute = what[1];
		std::string value = what[2];
		StyleParser::SetStyleAttributeFunc f = StyleParser::getSetter(attribute);
		if (!f)
			continue;
		f(outStyle, value);
	}
}
} // namespace

namespace sambag { namespace io {
//=============================================================================
// stream operators
//=============================================================================
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::svg::Style& style) {
	using namespace sambag::disco;
	std::string str;
	svg::AttributeParser::getWholeString(istr, str);
	parseStyleProperties(str, style);

	return istr;
}
}} // namespace(s)
