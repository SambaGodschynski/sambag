/*
 * PopupMenu.cpp
 *
 *  Created on: Mon Jun 11 11:19:42 2012
 *      Author: Johannes Unger
 */

#include "PopupMenu.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class PopupMenu
//=============================================================================
//-----------------------------------------------------------------------------
PopupMenu::PopupMenu() {
	setName("PopupMenu");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr PopupMenu::getComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<PopupMenu>();
}

}}} // namespace(s)
