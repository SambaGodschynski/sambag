/*
 * Line.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "Line.hpp"

namespace sambag { namespace disco { namespace graphicElements {

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
	cn->moveTo(p0); cn->lineTo(p1);
	cn->stroke();
}

}}}
