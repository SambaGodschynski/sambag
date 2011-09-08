/*
 * StyleParser.hpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#ifndef STYLEPARSER_HPP_
#define STYLEPARSER_HPP_

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
class StyleParser {
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
	static void parseColor(const std::string &str, ColorRGBA&);
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



#endif /* STYLEPARSER_HPP_ */
