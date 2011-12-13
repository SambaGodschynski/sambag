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
IPattern::Ptr SvgLinearGradient::createPattern(const Rectangle &rect) const {
	Point2D p0 = rect.min_corner();
	Point2D p1 = rect.max_corner();
	ALinearPattern::Ptr pattern = getDiscoFactory()->createLinearPattern(p0, p1);
	pattern->addColorStops(getColorStops());
	return pattern;
}

}}}
