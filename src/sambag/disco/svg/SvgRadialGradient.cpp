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
IPattern::Ptr SvgRadialGradient::createPattern() const {

	Coordinate cx=c.x().getValue(), cy=c.y().getValue(), 
			   fx=f.x().getValue(), fy=f.y().getValue(), _r=r.getValue();

	IRadialPattern::Ptr pattern = getDiscoFactory()->
		createRadialPattern(Point2D(fx,fy), 0, Point2D(cx,cy), _r);

	pattern->addColorStops(getColorStops());
	return pattern;
}
}}}
