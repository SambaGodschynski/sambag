/*
 * CheckBox.cpp
 *
 *  Created on: Mon Dec  3 12:58:05 2012
 *      Author: Johannes Unger
 */

#include "CheckBox.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class CheckBox
//=============================================================================
//-----------------------------------------------------------------------------
CheckBox::CheckBox() {
	setName("CheckBox");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr CheckBox::createComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<CheckBox>();
}
}}} // namespace(s)
