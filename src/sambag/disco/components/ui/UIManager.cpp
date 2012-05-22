/*
 * UIManager.cpp
 *
 *  Created on: Sun Jun  3 14:17:21 2012
 *      Author: Johannes Unger
 */

#include "UIManager.hpp"
#include <loki/Singleton.h>

namespace sambag { namespace disco { namespace components { namespace ui {

//=============================================================================
//  Class UIManager
//=============================================================================
//-----------------------------------------------------------------------------
void UIManager::installLookAndFeel(RootPane::Ptr root, ALookAndFeel::Ptr laf)
{
	lafMap[root] = laf;
	root->installLookAndFeel(laf);
}
//-----------------------------------------------------------------------------
ALookAndFeel::Ptr UIManager::getLookAndFeel(RootPane::Ptr root) const {
	LafMap::const_iterator it = lafMap.find(root);
	if (it!=lafMap.end()) {
		return it->second;
	}
	// TODO: return default look and feel
	return ALookAndFeel::Ptr();
}
//-----------------------------------------------------------------------------
UIManager & UIManager::instance() {
	return UIManagerHolder::Instance();
}
}}}} // namespace(s)
