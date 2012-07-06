/*
 * ScrollPane.cpp
 *
 *  Created on: Thu Jul  5 16:50:53 2012
 *      Author: Johannes Unger
 */

#include "ScrollPane.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ScrollPane
//=============================================================================
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VERTICAL_SCROLLBAR_POLICY =
		"vertical scrollbar policy";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_HORIZONTAL_SCROLLBAR_POLICY =
		"horizontal scrollbar policy";
//-----------------------------------------------------------------------------
ScrollPane::ScrollPane() {
	setName("ScrollPane");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr ScrollPane::getComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<ScrollPane>();
}
}}} // namespace(s)
