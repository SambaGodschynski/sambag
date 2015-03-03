/*
 * Button.cpp
 *
 *  Created on: Mon Jun  4 12:21:08 2012
 *      Author: Johannes Unger
 */

#include "Button.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Button
//=============================================================================
//-----------------------------------------------------------------------------
Button::Button() {
	setBackground(ColorRGBA(1,1,1));
	setForeground(ColorRGBA(0,0,0));
	setName("Button");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Button::createComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<Button>();
}
//-----------------------------------------------------------------------------
std::string Button::parameterString() const {
	return getText() + ", " + Super::parameterString();
}
}}} // namespace(s)
