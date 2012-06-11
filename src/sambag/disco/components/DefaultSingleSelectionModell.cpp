/*
 * DefaultSingleSelectionModell.cpp
 *
 *  Created on: Mon Jun 11 11:47:53 2012
 *      Author: Johannes Unger
 */

#include "DefaultSingleSelectionModell.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class DefaultSingleSelectionModell
//=============================================================================
//-----------------------------------------------------------------------------
void DefaultSingleSelectionModell::clearSelection() {
	using namespace sambag::com::events;
	selection = -1;
	EventSender<DefaultSingleSelectionModellChanged>::notifyListeners(
			this,
			DefaultSingleSelectionModellChanged(*this)
	);
}
//-----------------------------------------------------------------------------
int DefaultSingleSelectionModell::getSelectedIndex() const {
	return selection;
}
//-----------------------------------------------------------------------------
bool DefaultSingleSelectionModell::isSelected() const {
	return selection != -1;
}
//-----------------------------------------------------------------------------
void DefaultSingleSelectionModell::setSelectedIndex(int index) {
	using namespace sambag::com::events;
	selection = index;
	EventSender<DefaultSingleSelectionModellChanged>::notifyListeners(
			this,
			DefaultSingleSelectionModellChanged(*this)
	);
}

}}} // namespace(s)
