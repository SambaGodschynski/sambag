/*
 * Cirlce.cpp
 *
 *  Created on: 03.10.2011
 *      Author: samba
 */

#include "Arc.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// class Circle
//=============================================================================
//-----------------------------------------------------------------------------
void Arc::draw( IDrawContext::Ptr cn ) {

	Matrix tmp;
	cn->getMatrix(tmp);
	cn->translate(c);
	cn->scale(Point2D(1.0, r.y()/r.x()));

	if ( style.isFilled() ) {
		style.setFillStyle(cn);
		cn->arc(Point2D(), r.x(), 0, 2.0*M_PI);
		cn->fill();
	}
	if ( style.isStroked() ) {
		style.setStrokeStyle(cn);
		cn->arc(Point2D(), r.x(), 0, 2.0*M_PI);
		cn->identityMatrix();
		cn->transform(tmp);
		cn->stroke();
	}
	cn->identityMatrix();
	cn->transform(tmp);
}

}}}
