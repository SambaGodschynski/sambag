/*
 * SvgCircle.cpp
 *
 *  Created on: 04.10.2011
 *      Author: samba
 */

#include "SvgEllipse.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgEllipse
//=============================================================================
//-----------------------------------------------------------------------------
SvgEllipse::SvgEllipse() {
	using namespace sambag::disco::svg::graphicElements;
	ellipse = Arc::create();
}
//-----------------------------------------------------------------------------
SvgEllipse::~SvgEllipse() {
}
}}}
