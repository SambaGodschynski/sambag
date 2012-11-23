/*
 * DefaultTooltipManager.cpp
 *
 *  Created on: Fri Nov 23 14:43:39 2012
 *      Author: Johannes Unger
 */

#include "DefaultTooltipManager.hpp"


namespace sambag { namespace disco { namespace components {
typedef Loki::SingletonHolder<DefaultTooltipManager> DefaultTooltipManagerHolder;
//=============================================================================
//  Class DefaultTooltipManager
//=============================================================================
//-----------------------------------------------------------------------------
DefaultTooltipManager & DefaultTooltipManager::instance() {
	return DefaultTooltipManagerHolder::Instance();
}
//-----------------------------------------------------------------------------
int DefaultTooltipManager::getDismissDelay() const {
	return 0;
}
//-----------------------------------------------------------------------------
int DefaultTooltipManager::getInitialDelay() const {
	return 0;
}
//-----------------------------------------------------------------------------
int DefaultTooltipManager::getReshowDelay() const {
	return 0;
}
//-----------------------------------------------------------------------------
bool DefaultTooltipManager::isEnabled() const {
	return false;
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::registerComponent(AComponentPtr component)
{

}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setDismissDelay(int milliseconds) {

}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setEnabled(bool flag) {

}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setInitialDelay(int milliseconds) {

}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setReshowDelay(int milliseconds) {

}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::unregisterComponent(AComponentPtr component)
{

}
}}} // namespace(s)
