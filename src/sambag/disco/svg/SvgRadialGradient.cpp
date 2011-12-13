/*
 * SvgRadialGradient.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: samba
 */

#include "SvgRadialGradient.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgLinearGradient
//=============================================================================
//-----------------------------------------------------------------------------
APattern::Ptr SvgRadialGradient::createPattern(const Rectangle &rect) const {
	// cal. rect mid point
	Point2D c0 = rect.min_corner();
	Point2D tmp = rect.max_corner();
	boost::geometry::subtract_point(tmp, rect.min_corner());
	boost::geometry::divide_value(tmp, 2.0);
	boost::geometry::add_point(c0, tmp);

	// create pattern at (0,0) with r=witdh/2
	RadialPattern::Ptr pattern = RadialPattern::create(Point2D(0,0), 0, Point2D(0,0), rect.getWidth()/2.0);

	// translate and scale pattern space to fit
	Number ratio = rect.getWidth() / rect.getHeight();
	Matrix trans = scale2D(1., ratio);
	trans = boost::numeric::ublas::prod(trans, translate2D(-c0.x(), -c0.y()));
	pattern->setMatrix(trans);

	pattern->addColorStops(getColorStops());
	return pattern;
}
}}}
