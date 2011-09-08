/*
 * StyleParser.hpp
 *
 *  Created on: 14.10.2011
 *      Author: samba
 */

#ifndef DISCO_STYLEPARSER_HPP_
#define DISCO_STYLEPARSER_HPP_

#include "sambag/disco/graphicElements/Style.hpp"
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
			disco::graphicElements::Style& style,
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
	static SetStyleAttributeFunc getSetter(const std::string& str);
};

}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
extern std::istream & operator>>(std::istream&, sambag::disco::graphicElements::Style&);
#endif /* STYLEPARSER_HPP_ */
