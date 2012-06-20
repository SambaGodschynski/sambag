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
const std::string MenuItem::PROPERTY_DELAY = "menuItemDelay";
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
//-----------------------------------------------------------------------------
void MenuItem::processMouseEvent(events::MouseEvent event,
		const MenuElements & path, MenuSelectionManager &manager)
{
	AComponent::processMouseEvent(event);
}
//-----------------------------------------------------------------------------
void MenuItem::menuSelectionChanged(bool isIncluded) {
}
//-----------------------------------------------------------------------------
void MenuItem::getSubElements(MenuElements &out) const {
}
//-----------------------------------------------------------------------------
void MenuItem::setDelay(int newDelay) {
	int old = delay;
	delay = newDelay;
	firePropertyChanged(PROPERTY_DELAY, old, newDelay);
}

}}} // namespace(s)
