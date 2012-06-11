/*
 * Panel.cpp
 *
 *  Created on: Mon Jun 11 20:49:43 2012
 *      Author: Johannes Unger
 */

#include "Panel.hpp"
#include "FlowLayout.hpp"
#include <sambag/disco/svg/HtmlColors.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Panel
//=============================================================================
//-----------------------------------------------------------------------------
Panel::Panel(){
	setBackground(svg::HtmlColors::getColor("AliceBlue"));
	setForeground(ColorRGBA(0,0,0,1));
	setLayout(FlowLayout::create());
	setName("Panel");
}
}}} // namespace(s)
