/*
 * SvgRadialGradient.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: samba
 */

#include "SvgRadialGradient.hpp"
#include "sambag/disco/IDiscoFactory.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgLinearGradient
//=============================================================================
//-----------------------------------------------------------------------------
IPattern::Ptr SvgRadialGradient::createPattern(const Rectangle &rect) const {
	// cal. rect mid point
	Point2D c0 = rect.x0();
	Point2D tmp = rect.x1();
	boost::geometry::subtract_point(tmp, rect.x0());
	boost::geometry::divide_value(tmp, 2.0);
	boost::geometry::add_point(c0, tmp);

	// create pattern at (0,0) with r=witdh/2
	IRadialPattern::Ptr pattern = getDiscoFactory()->
		createRadialPattern(Point2D(0,0), 0, Point2D(0,0), rect.getWidth()/2.0);

	// translate and scale pattern space to fit
	Number ratio = rect.getWidth() / rect.getHeight();
	Matrix trans = scale2D(1., ratio);
	trans = boost::numeric::ublas::prod(trans, translate2D(-c0.x(), -c0.y()));
	pattern->setMatrix(trans);

	pattern->addColorStops(getColorStops());
	return pattern;
}
}}}
