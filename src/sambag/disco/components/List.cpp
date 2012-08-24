/*
 * List.cpp
 *
 *  Created on: Thu Jul  5 16:54:31 2012
 *      Author: Johannes Unger
 */

#include "List.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class List
//=============================================================================
//-----------------------------------------------------------------------------
List::List() {
	setName("List");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr List::createComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<List>();
}
}}} // namespace(s)
