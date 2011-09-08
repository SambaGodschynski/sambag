/*
 * PointContainer.cpp
 *
 *  Created on: 30.09.2011
 *      Author: samba
 */

#include "PointContainer.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
namespace pathInstruction {
//=============================================================================
bool isCubic ( InstructionOp op ) {
	return op == CURVETO_CUBIC_ABS || op == CURVETO_CUBIC_REL ||
		   op == CURVETO_CUBIC_SMOOTH_ABS || op == CURVETO_CUBIC_SMOOTH_REL;

}
bool isQuadratic ( InstructionOp op ) {
	return op == CURVETO_QUADRATIC_ABS || op == CURVETO_QUADRATIC_REL ||
		   op == CURVETO_QUADRATIC_SMOOTH_ABS || op == CURVETO_QUADRATIC_SMOOTH_REL;

}

}}}} // namespace
