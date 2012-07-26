/*
 * PathInstruction.hpp
 *
 *  Created on: 27.09.2011
 *      Author: samba
 */

#ifndef POINT_CONTAINER_HPP_
#define POINT_CONTAINER_HPP_

#include <list>
#include "sambag/com/Common.hpp"
#include "ostream"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
/**
 * Point container for the path element. Every point can be related to a
 * instruction tag.
 */
namespace pathInstruction {
//=============================================================================
//-----------------------------------------------------------------------------
enum InstructionOp {
	NONE,
	CLOSEPATH                    ,
	MOVETO_ABS                   ,
	MOVETO_REL                   ,
	LINETO_ABS                   ,
	LINETO_REL                   ,
	CURVETO_CUBIC_ABS            ,
	CURVETO_CUBIC_REL            ,
	CURVETO_QUADRATIC_ABS        ,
	CURVETO_QUADRATIC_REL        ,
	ARC_ABS                      ,
	ARC_REL                      ,
	LINETO_HORIZONTAL_ABS        ,
	LINETO_HORIZONTAL_REL        ,
	LINETO_VERTICAL_ABS          ,
	LINETO_VERTICAL_REL          ,
	CURVETO_CUBIC_SMOOTH_ABS     ,
	CURVETO_CUBIC_SMOOTH_REL     ,
	CURVETO_QUADRATIC_SMOOTH_ABS ,
	CURVETO_QUADRATIC_SMOOTH_REL
};
typedef std::list<sambag::com::Number> PointContainer;
typedef std::pair<PointContainer, InstructionOp> PathInstruction;
typedef std::list<PathInstruction> PathInstructions;
extern bool isCubic ( InstructionOp op );
extern bool isQuadratic ( InstructionOp op );

}}}}} // namespace

/**
 * for debugging.
 * @return svg path instructions
 */
extern std::ostream & operator<<(
		std::ostream &,
		const sambag::disco::svg::graphicElements::pathInstruction::PathInstructions&
);

#endif /* PATHINSTRUCTION_HPP_ */
