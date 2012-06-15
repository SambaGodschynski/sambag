/*
 * Menu.cpp
 *
 *  Created on: Mon Jun 11 06:32:54 2012
 *      Author: Johannes Unger
 */

#include "Menu.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Menu
//=============================================================================
//-----------------------------------------------------------------------------
void Menu::add(MenuItem::Ptr item) {
}
//-----------------------------------------------------------------------------
void Menu::add(AComponent::Ptr comp, size_t index) {
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Menu::getComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<Menu>();
}
}}} // namespace(s)
