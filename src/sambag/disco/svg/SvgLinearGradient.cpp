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
	Point2D p0(0,0);
	Point2D p1(100,0);
	ILinearPattern::Ptr pattern = getDiscoFactory()->createLinearPattern(p0, p1);
	pattern->addColorStops(getColorStops());
	return pattern;
}

}}}
