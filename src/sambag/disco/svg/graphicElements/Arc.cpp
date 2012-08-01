/*
 * Cirlce.cpp
 *
 *  Created on: 03.10.2011
 *      Author: samba
 */

#include "Arc.hpp"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
// class Circle
//=============================================================================
//-----------------------------------------------------------------------------
void Arc::draw( IDrawContext::Ptr cn ) {
	Point2D r;
	if (this->r.y().getType() == units::Unit::NONE) { // in case of circle
		// solve a single coord. has a different result to solve a point.
		r.x(this->r.x().solve(cn));
		r.y(r.x());
	} else {
		r = this->r.solve(cn);
	}
	Point2D c = this->c.solve(cn);
	Matrix tmp(3,3);
	cn->getMatrix(tmp);
	cn->translate(c);
	cn->scale(Point2D(1.0, r.y()/r.x()));

	if ( cn->isFilled() ) {
		cn->arc(Point2D(0,0), r.x(), 0, 2.0*M_PI);
		// reset transformation before fill (thx virginia)
		// (arc is drawn at (0,0) but a fill pattern has (c.x, c.y) bounds.
		cn->identityMatrix();
		cn->transform(tmp);
		cn->fill();
		if (!cn->isStroked()) // skip re- transform if possible
			return;
		// re- transform
		cn->translate(c);
		cn->scale(Point2D(1.0, r.y()/r.x()));
	}
	if ( cn->isStroked() ) {
		cn->arc(Point2D(0,0), r.x(), 0, 2.0*M_PI);
		// reset transformation before stroke
		cn->identityMatrix();
		cn->transform(tmp);
		cn->stroke();
		return;
	}
}
//-----------------------------------------------------------------------------
Rectangle Arc::getBoundingBox(IDrawContext::Ptr cn) const {
	Point2D _r;
	if (this->r.y().getType() == units::Unit::NONE) { // in case of circle
		// solve a single coord. has a different result to solve a point.
		_r.x(this->r.x().solve(cn));
		_r.y(_r.x());
	} else {
		_r = this->r.solve(cn);
	}
	Point2D p0 = c.solve(cn);
	boost::geometry::subtract_point(p0, _r);
	boost::geometry::multiply_value(_r, 2.);
	return Rectangle(p0, _r.x(), _r.y());
}
}}}}