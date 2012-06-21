/*
 * Menu.cpp
 *
 *  Created on: Mon Jun 11 06:32:54 2012
 *      Author: Johannes Unger
 */

#include "Menu.hpp"
#include "ui/ALookAndFeel.hpp"
#include "PopupMenu.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Menu
//=============================================================================
//-----------------------------------------------------------------------------
Menu::Menu() {
}
//-----------------------------------------------------------------------------
void Menu::constructorAlt() {
	popupMenu = PopupMenu::create(getPtr());
	popupMenu->setInvoker(getPtr());
}
//-----------------------------------------------------------------------------
void Menu::add(AComponent::Ptr comp, size_t index) {
	popupMenu->add(comp, index);
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Menu::getComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<Menu>();
}
//-----------------------------------------------------------------------------
void Menu::setPopupMenuVisible(bool b) {
	if (b) {
		Point2D p = getLocationOnScreen(Point2D());
		boost::geometry::add_point(p, Point2D(getWidth(), 0));
		popupMenu->showPopup(p);
	} else {
		popupMenu->hidePopup();
	}
}
//-----------------------------------------------------------------------------
bool Menu::isPopupMenuVisible() const {
	return popupMenu->isPopupVisible();
}
}}} // namespace(s)
