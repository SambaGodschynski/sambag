/*
 * Label.cpp
 *
 *  Created on: Tue Jun 12 17:37:30 2012
 *      Author: Johannes Unger
 */

#include "Label.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Label
//=============================================================================
//-----------------------------------------------------------------------------
const std::string Label::PROPERTY_LABELTEXT = "labeltext";
//-----------------------------------------------------------------------------
Label::Label() {
	setForeground(ColorRGBA(0,0,0));
	setName("Label");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Label::createComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<Label>();
}
//-----------------------------------------------------------------------------
void Label::setText(const std::string &txt) {
	std::string old = txt;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		text = txt;
	SAMBAG_END_SYNCHRONIZED
	firePropertyChanged(PROPERTY_LABELTEXT, old, txt);
	invalidate();
	redraw();

}
}}} // namespace(s)
