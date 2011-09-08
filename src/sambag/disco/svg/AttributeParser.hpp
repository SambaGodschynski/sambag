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
#include "sambag/disco/graphicElements/PointContainer.hpp"

namespace sambag { namespace disco { namespace svg {
using namespace sambag::com;
using namespace sambag::disco::graphicElements;
using sambag::disco::graphicElements::pathInstruction::PathInstructions;
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
	/**
	 * @param svg path command like M or z
	 * @return Instruction
	 */
	static pathInstruction::InstructionOp getPathInstruction(const std::string&);
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
extern std::istream & operator>>(
		std::istream&,
		sambag::disco::graphicElements::pathInstruction::PathInstructions &
);


#endif /* ATTRIBUTE_PARSER_HPP_ */
