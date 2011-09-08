/*
 * SvgCircle.cpp
 *
 *  Created on: 04.10.2011
 *      Author: samba
 */

#include "SvgCircle.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgCircle
//=============================================================================
//-----------------------------------------------------------------------------
SvgCircle::SvgCircle() {
	using namespace sambag::disco::graphicElements;
	circle = Arc::create();
}
//-----------------------------------------------------------------------------
SvgCircle::~SvgCircle() {
}
//-----------------------------------------------------------------------------
void SvgCircle::draw( IDrawContext::Ptr cn ) {
	if (!circle) return;
	AutoTransform at(getTransformMatrix(), cn);
	circle->draw(cn);
}

}}}
