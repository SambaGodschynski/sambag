/*
 * PathInstructions.cpp
 *
 *  Created on: 28.09.2011
 *      Author: samba
 */

#include "AttributeParser.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_inserter.hpp>
#include "sambag/com/Common.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
// Parser
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * created with "extract_html_colors.py"
 */
void AttributeParser::initPathInstr() {
	using namespace boost::assign;
	using namespace pathInstruction;
	insert(pathInstrMap)
		("z", CLOSEPATH)
		("Z", CLOSEPATH)
		("M", MOVETO_ABS)
		("m", MOVETO_REL)
		("L", LINETO_ABS)
		("l", LINETO_REL)
		("C", CURVETO_CUBIC_ABS)
		("c", CURVETO_CUBIC_REL)
		("Q", CURVETO_QUADRATIC_ABS)
		("q", CURVETO_QUADRATIC_REL)
		("A", ARC_ABS)
		("a", ARC_REL)
		("H", LINETO_HORIZONTAL_ABS)
		("h", LINETO_HORIZONTAL_REL)
		("V", LINETO_VERTICAL_ABS)
		("v", LINETO_VERTICAL_REL)
		("S", CURVETO_CUBIC_SMOOTH_ABS)
		("s", CURVETO_CUBIC_SMOOTH_REL)
		("T", CURVETO_QUADRATIC_SMOOTH_ABS)
		("t", CURVETO_QUADRATIC_SMOOTH_REL);

}
}}} // namespaces
