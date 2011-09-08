/*
 * Line.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "SvgLine.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgLine
//=============================================================================
//-----------------------------------------------------------------------------
SvgLine::SvgLine() {
	using namespace sambag::disco::graphicElements;
	line = Line::create();
}
//-----------------------------------------------------------------------------
SvgLine::~SvgLine() {
}
//-----------------------------------------------------------------------------
void SvgLine::draw( IDrawContext::Ptr cn ) {
	line->draw(cn);
}

}}}
