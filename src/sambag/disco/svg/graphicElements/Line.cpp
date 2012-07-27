/*
 * Line.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "Line.hpp"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {

//=============================================================================
// class Line
//=============================================================================
//-----------------------------------------------------------------------------
Line::Line() {
}
//-----------------------------------------------------------------------------
Line::~Line() {
}
//-----------------------------------------------------------------------------
void Line::draw( IDrawContext::Ptr cn ) {
	if ( !cn->isStroked() )
		return;
	cn->moveTo(p0.solve(cn)); cn->lineTo(p1.solve(cn));
	cn->stroke();
}

}}}}
