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
	using namespace sambag::disco::graphicElements;
	ellipse = Arc::create();
}
//-----------------------------------------------------------------------------
SvgEllipse::~SvgEllipse() {
}
//-----------------------------------------------------------------------------
void SvgEllipse::draw( IDrawContext::Ptr cn ) {
	if (!ellipse) return;
	AutoTransform at(getTransformMatrix(), cn);
	ellipse->draw(cn);
}

}}}
