/*
 * ColumnBrowser.cpp
 *
 *  Created on: Wed Sep 19 11:20:11 2012
 *      Author: Johannes Unger
 */

#include "ColumnBrowser.hpp"
#include "ui/ALookAndFeel.hpp"
#include "BorderLayout.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ColumnBrowser
//=============================================================================
//-----------------------------------------------------------------------------
ColumnBrowser::ColumnBrowser() {
	setName("ColumnBrowser");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr
ColumnBrowser::createComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<ColumnBrowser>();
}
}}} // namespace(s)
