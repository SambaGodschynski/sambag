/*
 * StyleParser.hpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#ifndef ATTRIBUTE_PARSER_HPP_
#define ATTRIBUTE_PARSER_HPP_

#include "sambag/com/Common.hpp"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <string>
#include <map>
#include "sambag/disco/Font.hpp"

namespace sambag { namespace disco { namespace svg {
using namespace sambag::com;
//=============================================================================
// parser
//=============================================================================
class AttributeParser {
private:
	//-------------------------------------------------------------------------
	typedef std::map<std::string, ColorRGBA> HtmlColors;
	//-------------------------------------------------------------------------
	static HtmlColors htmlColors;
	//-------------------------------------------------------------------------
	static void initHtmlColors();
public:
	//-------------------------------------------------------------------------
	/**
	 * Prepares string for further processing. such as trim, to_lower ...
	 * @param inout
	 * @return prepared string
	 */
	static std::string & prepareString(std::string &inout);
	//-------------------------------------------------------------------------
	/**
	 * Parses html string color to ColorRGBA.
	 * Can be color name or "#rrggbb" hex string
	 * @param str
	 * @param out_color
	 */
	static void parseColor(const std::string &str, ColorRGBA&);
	//-------------------------------------------------------------------------
	static void parseTransform(const std::string &str, Matrix&);
	//-------------------------------------------------------------------------
	static const ColorRGBA & getColorByHtmlName( const std::string &name );
};

}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
extern std::istream & operator>>(std::istream&, sambag::com::ColorRGBA&);
extern std::istream & operator>>(std::istream&, sambag::disco::Font::Weight&);
extern std::istream & operator>>(std::istream&, sambag::disco::Font::Slant&);
extern std::istream & operator>>(std::istream&, sambag::com::Matrix&);



#endif /* ATTRIBUTE_PARSER_HPP_ */
