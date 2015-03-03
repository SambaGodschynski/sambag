/*
 * ITooltipManager.cpp
 *
 *  Created on: Fri Nov 23 14:14:54 2012
 *      Author: Johannes Unger
 */

#include "ITooltipManager.hpp"
#include "DefaultTooltipManager.hpp"

namespace sambag { namespace disco { namespace components {
namespace {
	TooltipManagerCreator __creator;
} // namespace(s)
//-----------------------------------------------------------------------------
void registerTooltipManager(const TooltipManagerCreator &creator) {
	__creator = creator;
}
//-----------------------------------------------------------------------------
ITooltipManager & getTooltipManager() {
	if (__creator)
		return __creator();
	return DefaultTooltipManager::instance();
}
//-----------------------------------------------------------------------------
const TooltipManagerCreator & getTooltipManagerCreator() {
	return __creator;
}
}}} // namespace(s)
