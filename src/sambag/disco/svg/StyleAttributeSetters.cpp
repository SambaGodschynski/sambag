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
void setFillColor( Style &style, const std::string &value ) {
	std::stringstream is;
	is<<value;
	ColorRGBA col;
	is>>col;
	style.fillColor(col);
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
