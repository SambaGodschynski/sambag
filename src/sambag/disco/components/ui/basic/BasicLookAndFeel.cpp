/*
 * BasicLookAndFeel.cpp
 *
 *  Created on: Tue Jun  5 07:33:20 2012
 *      Author: Johannes Unger
 */

#include "BasicLookAndFeel.hpp"
// button
#include "BasicButtonUI.hpp"
#include "BasicMenuItemUI.hpp"
#include "BasicPopupMenuUI.hpp"
#include "BasicLabelUI.hpp"
#include "BasicMenuUI.hpp"
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/components/MenuItem.hpp>
#include <sambag/disco/components/PopupMenu.hpp>
#include <sambag/disco/components/Label.hpp>
#include <sambag/disco/components/Menu.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
//  Class BasicLookAndFeel
//=============================================================================
//-----------------------------------------------------------------------------
BasicLookAndFeel::BasicLookAndFeel() {
	installComponents();
}
//-----------------------------------------------------------------------------
void BasicLookAndFeel::installComponents() {
	registerComponentUI<Button, BasicButtonUI<Button::Modell> >();
	registerComponentUI<MenuItem, BasicMenuItemUI<MenuItem::Modell> >();
	registerComponentUI<PopupMenu, BasicPopupMenuUI<PopupMenu::Modell> >();
	registerComponentUI<Label, BasicLabelUI>();
	registerComponentUI<Menu, BasicMenuUI<Menu::Modell> >();
}

}}}}} // namespace(s)
