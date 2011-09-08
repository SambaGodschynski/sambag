/*
 * SvgRect.cpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#include "SvgRect.hpp"
namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgRect
//=============================================================================
//-----------------------------------------------------------------------------
SvgRect::SvgRect() {
	using namespace sambag::disco::graphicElements;
	rect = Rect::create();
}
//-----------------------------------------------------------------------------
SvgRect::~SvgRect() {
}
}}}
