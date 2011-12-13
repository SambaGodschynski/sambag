/*
 * SvgLinearGradient.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: samba
 */

#include "SvgLinearGradient.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgLinearGradient
//=============================================================================
//-----------------------------------------------------------------------------
APattern::Ptr SvgLinearGradient::createPattern(const Rectangle &rect) const {
	Point2D p0 = rect.min_corner();
	Point2D p1 = rect.max_corner();
	LinearPattern::Ptr pattern = LinearPattern::create(p0, p1);
	pattern->addColorStops(getColorStops());
	return pattern;
}

}}}
