/*
 * Panel.cpp
 *
 *  Created on: Mon Jun 11 20:49:43 2012
 *      Author: Johannes Unger
 */

#include "Panel.hpp"
#include "FlowLayout.hpp"
#include <sambag/disco/svg/HtmlColors.hpp>
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Panel
//=============================================================================
//-----------------------------------------------------------------------------
Panel::Panel(){
	setLayout(FlowLayout::create());
	setOpaque(true);
	setName("Panel");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Panel::getComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<Panel>();
}
}}} // namespace(s)
