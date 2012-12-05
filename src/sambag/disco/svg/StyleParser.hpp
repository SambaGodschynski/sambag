/*
 * StyleParser.hpp
 *
 *  Created on: 14.10.2011
 *      Author: samba
 */

#ifndef DISCO_STYLEPARSER_HPP_
#define DISCO_STYLEPARSER_HPP_

#include "graphicElements/Style.hpp"
#include <list>
#include <map>
namespace sambag { namespace disco { namespace svg {
using namespace sambag::disco;
//=============================================================================
// parser
//=============================================================================
class StyleParser {
public:
	//-------------------------------------------------------------------------
	/**
	 * Set StyleAttribute function type
	 * @param style
	 * @param styleValue
	 */
	typedef void (*SetStyleAttributeFunc) (
			disco::svg::graphicElements::Style& style,
			const std::string& styleValue
	);
private:
	//-------------------------------------------------------------------------
	typedef std::map<std::string, SetStyleAttributeFunc> SetterMap;
	//-------------------------------------------------------------------------
	static SetterMap attr2Setter;
	//-------------------------------------------------------------------------
	static void initSetterMap();
public:
	//-------------------------------------------------------------------------
	/**
	 * @param style name
	 * @return style setter method
	 */
	static SetStyleAttributeFunc getSetter(const std::string& str);
};

}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
/**
 * sets css style properties into a Style object.
 * e.g.: "stroke: black; fill: yellow; ..."
 * @param in_stream
 * @param out_Style
 * @return stream
 */
extern std::istream & operator>>(std::istream&, sambag::disco::svg::graphicElements::Style&);
//-----------------------------------------------------------------------------
inline sambag::disco::svg::graphicElements::Style 
createStyle(const std::string &str) 
{
	
	sambag::disco::svg::graphicElements::Style res;
	res.font(sambag::disco::Font()); // TODO: why has sytle no std font?
	std::stringstream ss;
	ss<<str;
	ss>>res;
	return res;
}

#endif /* STYLEPARSER_HPP_ */
