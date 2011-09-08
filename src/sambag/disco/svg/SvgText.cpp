/*
 * SvgText.cpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#include "SvgText.hpp"
namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgText
//=============================================================================
//-----------------------------------------------------------------------------
SvgText::SvgText() {
	using namespace sambag::disco::graphicElements;
	text = Text::create();
}
//-----------------------------------------------------------------------------
SvgText::~SvgText() {
}
}}}

