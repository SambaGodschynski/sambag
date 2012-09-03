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
	resetUIPorpertyCache();
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
//-----------------------------------------------------------------------------
ArbitraryType::Ptr UIManager::getProperty(const std::string &key) const
{
	PropertyMap::const_iterator it = propertyMap.find(key);
	if (it==propertyMap.end())
		return ArbitraryType::Ptr();
	return it->second;
}
//-----------------------------------------------------------------------------
void UIManager::putPropertyImpl(const std::string &key,
		ArbitraryType::Ptr value)
{
	PropertyMap::iterator it = propertyMap.find(key);
	if (it==propertyMap.end()) {
		if (!value)
			return;
		bool inserted;
		boost::tie(it, inserted) =
			propertyMap.insert(std::make_pair(key, value));
		SAMBAG_ASSERT(inserted);
		return;
	}
	ArbitraryType::Ptr old = it->second;
	if (!value) {
		propertyMap.erase(it);
		return;
	}
	it->second = value;
}
}}}} // namespace(s)
