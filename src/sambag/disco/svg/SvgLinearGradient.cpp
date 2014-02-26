/*
 * SvgLinearGradient.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: samba
 */

#include "SvgLinearGradient.hpp"
#include "sambag/disco/IDiscoFactory.hpp"
namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgLinearGradient
//=============================================================================
//-----------------------------------------------------------------------------
IPattern::Ptr SvgLinearGradient::createPattern() const {
	Point2D p0(x1.x().getValue(),x1.y().getValue());
	Point2D p1(x2.x().getValue(),x2.y().getValue());
	ILinearPattern::Ptr pattern = getDiscoFactory()->createLinearPattern(p0, p1);
	pattern->addColorStops(getColorStops());
	return pattern;
}

}}}
