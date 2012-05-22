/*
 * ComponentUI.cpp
 *
 *  Created on: Wed May 23 14:41:08 2012
 *      Author: Johannes Unger
 */

#include "AComponentUI.hpp"
#include <sambag/disco/components/AComponent.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {
//=============================================================================
//  Class AComponentUI
//=============================================================================
//-----------------------------------------------------------------------------
bool AComponentUI::contains(AComponent::Ptr c, const Point2D &p) {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
int AComponentUI::getBaseline(AComponent::Ptr c, int width, int height) {
	SAMBA_LOG_NOT_YET_IMPL();
	return -1;
}
//-----------------------------------------------------------------------------
Dimension AComponentUI::getMaximumSize(AComponent::Ptr c) {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Dimension AComponentUI::getMinimumSize(AComponent::Ptr c) {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Dimension AComponentUI::getPreferredSize(AComponent::Ptr c) {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
void AComponentUI::installUI(AComponent::Ptr c) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponentUI::draw(IDrawContext::Ptr cn, AComponent::Ptr c) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponentUI::uninstallUI(AComponent::Ptr c) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AComponentUI::update(IDrawContext::Ptr cn, AComponent::Ptr c){
	SAMBA_LOG_NOT_YET_IMPL();
}

}}}} // namespace(s)
