/*
 * Menu.cpp
 *
 *  Created on: Mon Jun 11 06:32:54 2012
 *      Author: Johannes Unger
 */

#include "Menu.hpp"
#include "ui/ALookAndFeel.hpp"
#include "PopupMenu.hpp"
#include "WindowToolkit.hpp"
#include "ui/UIManager.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Menu
//=============================================================================
//-----------------------------------------------------------------------------
Menu::Menu() {
	setDelay(500);
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
		Point2D p = getLocationOnScreen(computePopupLocation());
		popupMenu->showPopup(p);
	} else {
		popupMenu->hidePopup();
	}
}
//-----------------------------------------------------------------------------
bool Menu::isPopupMenuVisible() const {
	return popupMenu->isPopupVisible();
}
//-----------------------------------------------------------------------------
Point2D Menu::computePopupLocation() const {
	const ui::UIManager &uim = ui::UIManager::instance();
	Coordinate x;
	Coordinate y;
	PopupMenu::Ptr pm = getPopupMenu();
	// Figure out the sizes needed to caclulate the menu position
	Dimension s = getSize();
	Dimension pmSize = pm->getSize();
	// For the first time the menu is popped up,
	// the size has not yet been initiated
	if (pmSize.width() == 0) {
		pmSize = pm->getPreferredSize();
	}
	Point2D position = getLocationOnScreen(Point2D());
	Dimension screenBounds = getWindowToolkit()->getScreenSize();


	AContainerPtr parent = getParent();
	PopupMenu::Ptr parentPop = boost::shared_dynamic_cast<PopupMenu>(parent);
	if (parent) {
		// We are a submenu (pull-right)
		Coordinate xOffset = 0;
		uim.getProperty("Menu.submenuPopupOffsetX", xOffset);
		Coordinate yOffset = 0;
		uim.getProperty("Menu.submenuPopupOffsetY", yOffset);
		// First determine x:
		x = s.width() + xOffset; // Prefer placement to the right
		if (position.x() + x + pmSize.width() >= screenBounds.width() &&
		// popup doesn't fit - place it wherever there's more room
				screenBounds.width() - s.width() < 2. * position.x())
		{

			x = 0 - xOffset - pmSize.width();
		}
		// Then the y:
		y = yOffset; // Prefer dropping down
		if (position.y() + y + pmSize.height() >= screenBounds.height() &&
		// popup doesn't fit - place it wherever there's more room
				screenBounds.height() - s.height() < 2. * position.y() )
		{

			y = s.height() - yOffset - pmSize.height();
		}
	} else {
		// We are a toplevel menu (pull-down)
		Coordinate xOffset = 0;
		uim.getProperty("Menu.menuPopupOffsetX", xOffset);
		Coordinate yOffset = 0;
		uim.getProperty("Menu.menuPopupOffsetY", yOffset);

		// First determine the x:
		x = xOffset; // Extend to the right
		if (position.x() + x + pmSize.width() >= screenBounds.width() &&
		// popup doesn't fit - place it wherever there's more room
				screenBounds.width() - s.width() < 2. * position.x())
		{

			x = s.width() - xOffset - pmSize.width();
		}
		// Then the y:
		y = s.height() + yOffset; // Prefer dropping down
		if (position.y() + y + pmSize.height() >= screenBounds.height() &&
		// popup doesn't fit - place it wherever there's more room
				screenBounds.height() - s.height() < 2. * position.y())
		{

			y = 0 - yOffset - pmSize.height(); // Otherwise drop 'up'
		}
	}
	return Point2D(x, y);
}
}}} // namespace(s)
