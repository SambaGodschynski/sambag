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
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Button::getComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<Button>();
}
}}} // namespace(s)
