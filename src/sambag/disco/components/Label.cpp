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
const std::string Label::PROPERTY_ICON = "icon";
//-----------------------------------------------------------------------------
Label::Label() {
	setForeground(ColorRGBA(0,0,0));
	setName("Label");
	setOpaque(true);
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
//-----------------------------------------------------------------------------
ISurface::Ptr Label::getIcon() const {
	return icon;
}
//-----------------------------------------------------------------------------
void Label::setIcon(ISurface::Ptr icon) {
	ISurface::Ptr old = this->icon;
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		this->icon = icon;
	SAMBAG_END_SYNCHRONIZED
	firePropertyChanged(PROPERTY_ICON, old, icon);
	invalidate();
	redraw();
}
}}} // namespace(s)
