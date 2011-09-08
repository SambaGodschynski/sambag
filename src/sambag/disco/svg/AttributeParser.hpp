/*
 * StyleParser.hpp
 *
 *  Created on: 21.09.2011
 *      Author: samba
 */

#ifndef ATTRIBUTE_PARSER_HPP_
#define ATTRIBUTE_PARSER_HPP_

#include "sambag/com/Common.hpp"
#include "sambag/disco/Font.hpp"
#include "sambag/disco/graphicElements/PointContainer.hpp"

namespace sambag { namespace disco { namespace svg {
using namespace sambag::com;
using namespace sambag::disco::graphicElements;
using sambag::disco::graphicElements::pathInstruction::PathInstructions;
using sambag::disco::graphicElements::pathInstruction::PointContainer;
//=============================================================================
// parser
//=============================================================================
class AttributeParser {
private:
	//-------------------------------------------------------------------------
	typedef std::map<std::string, ColorRGBA> HtmlColors;
	//-------------------------------------------------------------------------
	typedef std::map<std::string, pathInstruction::InstructionOp> PathInstrMap;
	//-------------------------------------------------------------------------
	static HtmlColors htmlColors;
	//-------------------------------------------------------------------------
	static PathInstrMap pathInstrMap;
	//-------------------------------------------------------------------------
	static void initHtmlColors();
	//-------------------------------------------------------------------------
	static void initPathInstr();
public:
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
	static void parsePathInstructions(const std::string &str, PathInstructions&);
	//-------------------------------------------------------------------------
	static void parseOpacity(const std::string &str, Number &v);
	//-------------------------------------------------------------------------
	static void parsePointContainer(const std::string &str, PointContainer&);
	//-------------------------------------------------------------------------
	/**
	 * Maps from a string to PathInstruction enum.
	 * @param svg path command like M or z
	 * @return Instruction
	 */
	static pathInstruction::InstructionOp getPathInstruction(const std::string&);
	//-------------------------------------------------------------------------
	static const ColorRGBA & getColorByHtmlName( const std::string &name );
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
	static Number string2Number(const std::string &str);
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
extern std::istream & operator>>(std::istream&, sambag::com::ColorRGBA&);
extern std::istream & operator>>(std::istream&, sambag::disco::Font::Weight&);
extern std::istream & operator>>(std::istream&, sambag::disco::Font::Slant&);
extern std::istream & operator>>(std::istream&, sambag::com::Matrix&);
extern std::istream & operator>>(
		std::istream&,
		sambag::disco::graphicElements::pathInstruction::PathInstructions &
);
extern std::istream & operator>>(
		std::istream&,
		sambag::disco::graphicElements::pathInstruction::PointContainer &
);


#endif /* ATTRIBUTE_PARSER_HPP_ */
