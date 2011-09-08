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
}}} // namespaces
namespace {
using namespace sambag::disco;
//-----------------------------------------------------------------------------
/**
 * parses style rules eg.: "stroke: red; stroke-width:2px;"
 * @param rules
 * @param outStyle
 */
void parseStyleRules( const std::string &rules, graphicElements::Style& outStyle) {
	using namespace sambag::disco::svg;
	if (rules.length()==0) return;
	std::string inStr = rules;
	AttributeParser::prepareString(inStr, false);
	// extract all matches
	std::string::const_iterator begin = inStr.begin();
	std::string::const_iterator end = inStr.end();
	boost::match_results<std::string::const_iterator> what;
	for ( ;
		regex_search(begin, end, what, boost::regex("([a-zA-Z-]+)\\s*:\\s*([a-zA-Z0-9#.\"':/ ]+)\\s*;"));
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
//=============================================================================
// stream operators
//=============================================================================
//-----------------------------------------------------------------------------
std::istream & operator>>(std::istream& istr, sambag::disco::graphicElements::Style& style) {
	using namespace sambag::disco;
	std::string str;
	svg::AttributeParser::getWholeString(istr, str);
	parseStyleRules(str, style);

	return istr;
}

