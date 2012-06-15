/*
 * MenuSelectionManager.cpp
 *
 *  Created on: Thu Jun 14 13:23:23 2012
 *      Author: Johannes Unger
 */

#include "MenuSelectionManager.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class MenuSelectionManager
//=============================================================================
//-----------------------------------------------------------------------------.
MenuSelectionManager & MenuSelectionManager::defaultManager() {
	return Loki::SingletonHolder<MenuSelectionManager>::Instance();
}
///////////////////////////////////////////////////////////////////////////////
}}} // namespace(s)
