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
Line::Line(const Point2D &p0, const Point2D &p1) : p0(p0), p1(p1) {
	style.isStroked = true;
}
//-----------------------------------------------------------------------------
Line::~Line() {
}
//-----------------------------------------------------------------------------
void Line::draw( IDrawContext::Ptr cn ) {
	if (!style.isStroked)
		return;
	style.setStrokeStyle(cn);
	cn->moveTo(p0); cn->lineTo(p1);
	cn->stroke();
}

}}}
