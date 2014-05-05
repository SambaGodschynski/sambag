/*
 * StyleParser.hpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#ifndef ATTRIBUTE_PARSER_HPP_
#define ATTRIBUTE_PARSER_HPP_

#include "sambag/disco/Geometry.hpp"
#include "sambag/com/Common.hpp"
#include "sambag/disco/Font.hpp"
#include "sambag/disco/ColorRGBA.hpp"
#include "sambag/disco/Dash.hpp"
#include "graphicElements/PointContainer.hpp"
#include "sambag/disco/IDrawContext.hpp"
#include <map>
#include "sambag/math/Matrix.hpp"
#include "units/Units.hpp"
#include "SvgPatternMatrix.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/assign/list_inserter.hpp>
#include <vector>
#include <sstream>
#include <math.h>
#include <algorithm>

namespace sambag { namespace disco { namespace svg {
using namespace sambag::com;
using namespace sambag::disco::svg::graphicElements;
using sambag::disco::svg::graphicElements::pathInstruction::PathInstructions;
using sambag::disco::svg::graphicElements::pathInstruction::PointContainer;
//=============================================================================
// parser
//=============================================================================
class AttributeParser {
private:
	//-------------------------------------------------------------------------
	typedef std::map<std::string, pathInstruction::InstructionOp> PathInstrMap;
	//-------------------------------------------------------------------------
	static PathInstrMap pathInstrMap;
	//-------------------------------------------------------------------------
	static void initPathInstr();
public:
	//-------------------------------------------------------------------------
	/**
	 * @param url
	 * @return content of "url(CONTENT)" respectively an empty string.
	 */
	static std::string getUrl(const std::string &url);
	//-------------------------------------------------------------------------
	/**
	 * Parses html string color to ColorRGBA.
	 * Can be color name or "#rrggbb" or "#rgb" hex string.
	 * (see: see http://www.w3.org/TR/CSS2/syndata.html#color-units )
	 * @param str
	 * @param out_color
	 */
	static void parseColor(const std::string &str, ColorRGBA&);
	//-------------------------------------------------------------------------
        static void parseTransform(const std::string &str, sambag::math::Matrix&, bool inverse=false);
	//-------------------------------------------------------------------------
	static void parseCoordinate(const std::string &str, Coordinate&);
	//-------------------------------------------------------------------------
	static void parseUnit(const std::string &str, units::Unit&);
	//---------------------------------------------------s----------------------
	static void parsePathInstructions(const std::string &str, PathInstructions&);
	//-------------------------------------------------------------------------
	static void parseOpacity(const std::string &str, Number &v);
	//-------------------------------------------------------------------------
	static void parsePointContainer(const std::string &str, PointContainer&);
	//-------------------------------------------------------------------------
	static void parseViewBox(const std::string &str, Rectangle&);
	//-------------------------------------------------------------------------
	static Dash::Ptr parseDashArray(const std::string &str);
	//-------------------------------------------------------------------------
	/**
	 * Maps from a string to PathInstruction enum.
	 * @param svg path command like M or z
	 * @return Instruction
	 */
	static pathInstruction::InstructionOp getPathInstruction(const std::string&);
	//-------------------------------------------------------------------------
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>helper
	/**
	 * Prepares string for further processing. such as trim, to_lower ...
	 * @param inout
	 * @return prepared string
	 */
	static std::string & prepareString(std::string &inout, bool toLower = true);
	//-------------------------------------------------------------------------
	static int hex2Int ( const std::string &hex);
	//-------------------------------------------------------------------------
	/**
	 * copies whole istream content to out string.
	 * @param istr
	 * @param out
	 */
	static void getWholeString( std::istream& istr, std::string &out );
	//-------------------------------------------------------------------------
	/**
	 * @param str
	 * @param errVal
	 * @return numbervalue of str or errVal if conversion fails.
	 */
	static Number string2Number(const std::string &str, const Number &errVal);
	//-------------------------------------------------------------------------
	/**
	 * Converts Container<string>:in to Container<Number>:out.
	 * out will be empty if one convertion failed.
	 * @param in
	 * @param out
	 */
	template <typename StrContainer, typename NumberContainer>
	static void strings2Numbers(const StrContainer &in, NumberContainer &out);
	/**
	 * gets values string such as "3, 4.5, 3" into Container of Numbers
	 * @param values
	 * @param out
	 */
	template< typename Container >
	static void getValuesFromString( const std::string &_values, Container &out );
};

}}} // namespaces

//=============================================================================
// stream operators
//=============================================================================
extern std::istream & operator>>(std::istream&, sambag::disco::svg::units::Unit&);
extern std::istream & operator>>(std::istream&, sambag::disco::ColorRGBA&);
extern std::istream & operator>>(std::istream&, sambag::disco::Font::Weight&);
extern std::istream & operator>>(std::istream&, sambag::disco::Font::Slant&);
extern std::istream & operator>>(std::istream&, sambag::math::Matrix&);
extern std::istream & operator>>(std::istream&, sambag::disco::svg::SvgPatternMatrix&);
extern std::istream & operator>>(
		std::istream&,
		sambag::disco::svg::graphicElements::pathInstruction::PathInstructions &
);
extern std::istream & operator>>(
	std::istream&,
	sambag::disco::svg::graphicElements::pathInstruction::PointContainer &
);
extern std::istream & operator>>(
	std::istream&,
	sambag::disco::IDrawContext::LineCapStyle &v
);
extern std::istream & operator>>(
	std::istream&,
	sambag::disco::IDrawContext::LineJoin &v
);

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// template impl.
//=============================================================================
//-----------------------------------------------------------------------------
template <typename StrContainer, typename NumberContainer>
void AttributeParser::strings2Numbers(const StrContainer &in, NumberContainer &out) {
	boost_for_each( const std::string &str, in ) {
		if (str.length()==0)
			continue;
		std::stringstream ss;
		ss<<str;
		Number n;
		ss>>n;
		if (ss.fail() || !ss.eof()) {
			out.clear();
			return;
		}
		out.push_back(n);
	}
}
template< typename Container >
void AttributeParser::getValuesFromString( const std::string &_values, Container &out ) {
	std::stringstream ss;
	ss<<_values;
	while(!ss.eof()) {
		typename Container::value_type value;
		ss>>value;
		if (ss.fail()) {
			char dump;
			ss.clear();
			ss>>dump; 
			continue;
		}
		out.push_back(value);
	}

}
}}} // namespaces


#endif /* ATTRIBUTE_PARSER_HPP_ */
