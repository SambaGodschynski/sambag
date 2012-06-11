/*
 * MenuItem.cpp
 *
 *  Created on: Mon Jun 11 06:33:02 2012
 *      Author: Johannes Unger
 */

#include "MenuItem.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class MenuItem
//=============================================================================
//-----------------------------------------------------------------------------
MenuItem::MenuItem() {
	setBackground(ColorRGBA(1,1,1));
	setForeground(ColorRGBA(0,0,0));
	setName("MenuItem");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr MenuItem::getComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<MenuItem>();
}

}}} // namespace(s)
