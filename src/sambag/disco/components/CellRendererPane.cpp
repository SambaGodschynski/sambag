/*
 * CellRendererPane.cpp
 *
 *  Created on: Wed Sep  5 12:05:09 2012
 *      Author: Johannes Unger
 */

#include "CellRendererPane.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class CellRendererPane
//=============================================================================
//-----------------------------------------------------------------------------
CellRendererPane::CellRendererPane() {
	setName("CellRendererPane");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr
CellRendererPane::getComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<CellRendererPane>();
}
}}} // namespace(s)
