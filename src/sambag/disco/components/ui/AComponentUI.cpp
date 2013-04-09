/*
 * ComponentUI.cpp
 *
 *  Created on: Wed May 23 14:41:08 2012
 *      Author: Johannes Unger
 */

#include "AComponentUI.hpp"
#include <sambag/disco/components/AComponent.hpp>
#include "UIManager.hpp"
#include <sambag/disco/DiscoHelper.hpp>
#include <sambag/disco/svg/graphicElements/Style.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {
//=============================================================================
//  Class AComponentUI
//=============================================================================
//-----------------------------------------------------------------------------
bool AComponentUI::contains(AComponent::Ptr c, const Point2D &p) {
	if (!c)
		return false;
	const Coordinate &width = c->getBounds().getWidth();
	const Coordinate &height = c->getBounds().getHeight();
	return (p.x() >= 0) && (p.x() < width) && (p.y() >= 0) && (p.y() < height);
}
//-----------------------------------------------------------------------------
int AComponentUI::getBaseline(AComponentPtr c, 
		const Coordinate &width, const Coordinate &height)
{
	SAMBA_LOG_NOT_YET_IMPL();
	return -1;
}
//-----------------------------------------------------------------------------
Dimension AComponentUI::getMaximumSize(AComponent::Ptr c) {
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
Dimension AComponentUI::getMinimumSize(AComponent::Ptr c) {
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
Dimension AComponentUI::getPreferredSize(AComponent::Ptr c) {
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
void AComponentUI::installUI(AComponent::Ptr c) {
	UIManager &m = getUIManager();
	svg::graphicElements::Style style;
	m.getProperty("global.style", style);
	if (!c->isFontSet())
		c->setFont(style.font());
	if (!c->isForegroundSet())
		c->setForeground(style.strokePattern());
	if (!c->isBackgroundSet())
		c->setBackground(style.fillPattern());
}
//-----------------------------------------------------------------------------
void AComponentUI::draw(IDrawContext::Ptr cn, AComponent::Ptr c) {
}
//-----------------------------------------------------------------------------
void AComponentUI::uninstallUI(AComponent::Ptr c) {
}
//-----------------------------------------------------------------------------
void AComponentUI::update(IDrawContext::Ptr cn, AComponent::Ptr c){
	if (c->isOpaque()) {
		IPattern::Ptr pat = c->getBackgroundPattern();
		Rectangle r(0, 0, c->getWidth(), c->getHeight());
		if (isGradient(pat)) {
			alignPattern(cn, pat, r);
		}
		cn->setFillPattern(pat);
		cn->rect(r);
		cn->fill();
	}
	draw(cn, c);
}

}}}} // namespace(s)
