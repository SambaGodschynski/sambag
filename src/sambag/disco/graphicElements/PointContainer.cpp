/*
 * PointContainer.cpp
 *
 *  Created on: 30.09.2011
 *      Author: samba
 */

#include "PointContainer.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
namespace pathInstruction {
//=============================================================================
//-----------------------------------------------------------------------------
bool isCubic ( InstructionOp op ) {
	return op == CURVETO_CUBIC_ABS || op == CURVETO_CUBIC_REL ||
		   op == CURVETO_CUBIC_SMOOTH_ABS || op == CURVETO_CUBIC_SMOOTH_REL;

}
//-----------------------------------------------------------------------------
bool isQuadratic ( InstructionOp op ) {
	return op == CURVETO_QUADRATIC_ABS || op == CURVETO_QUADRATIC_REL ||
		   op == CURVETO_QUADRATIC_SMOOTH_ABS || op == CURVETO_QUADRATIC_SMOOTH_REL;

}

}}}} // namespace

namespace {
using namespace sambag::disco::graphicElements::pathInstruction;
using namespace sambag::com;
/**
 * @param op
 * @return InstructionOp related string.
 */
std::string getInstructionAsString(InstructionOp op) {
	switch(op) {
		case CLOSEPATH: return "z";
		case MOVETO_ABS: return "M";
		case MOVETO_REL: return "m";
		case LINETO_ABS: return "L";
		case LINETO_REL: return "l";
		case CURVETO_CUBIC_ABS: return "C";
		case CURVETO_CUBIC_REL: return "c";
		case CURVETO_QUADRATIC_ABS: return "Q";
		case CURVETO_QUADRATIC_REL: return "q";
		case ARC_ABS: return "A";
		case ARC_REL: return "a";
		case LINETO_HORIZONTAL_ABS: return "H";
		case LINETO_HORIZONTAL_REL: return "h";
		case LINETO_VERTICAL_ABS: return "V";
		case LINETO_VERTICAL_REL: return "v";
		case CURVETO_CUBIC_SMOOTH_ABS: return "S";
		case CURVETO_CUBIC_SMOOTH_REL: return "s";
		case CURVETO_QUADRATIC_SMOOTH_ABS: return "T";
		case CURVETO_QUADRATIC_SMOOTH_REL: return "t";
		case NONE: break;
	}
	return "?";
}
}

//-----------------------------------------------------------------------------
extern std::ostream & operator<<(
	std::ostream &os,
	const sambag::disco::graphicElements::pathInstruction::PathInstructions& pIs)
{
	using namespace sambag::disco::graphicElements::pathInstruction;
	using namespace sambag::com;
	os<<"<path d=\"";
	boost_for_each(const PathInstruction &pi, pIs) {
		os<<getInstructionAsString(pi.second);
		boost_for_each(const Number &v, pi.first) {
			os<<v<<", ";
		}
		os<<std::endl;
	}
	os<<"\"/>"<<std::endl;
	return os;

}
