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
	Super::setName("CellRendererPane");
}
//-----------------------------------------------------------------------------
void CellRendererPane::postConstructor() {
	Super::postConstructor();
	setLayout(ALayoutManagerPtr());
	setVisible(false);
}
//-----------------------------------------------------------------------------
AComponent::Ptr CellRendererPane::add(const AComponentSharedOrWeak &x,
		int index, com::ArbitraryType::Ptr constraint)
{
	if (x->getParent() == getPtr()) {
		return AComponent::Ptr();
	} else {
		return Super::add(x, index, constraint);
	}
}
//-----------------------------------------------------------------------------
void CellRendererPane::drawComponent(IDrawContext::Ptr cn, AComponentPtr c,
		AContainerPtr p, const Rectangle &bounds, bool shouldValidate)
{
	if (!c) {
		if (p) {
			cn->setFillColor(p->getBackground());
			cn->rect(bounds);
			cn->fill();
		}
		return;
	}
	if (c->getParent() != getPtr()) {
		Super::add(c);
	}
	c->setBounds(bounds);
	if (shouldValidate) {
		c->validate();
	}

	cn->save();
	c->draw(cn);
	cn->restore();
	c->setBounds(Rectangle(-bounds.getWidth(), -bounds.getHeight(), 0, 0));
}
}}} // namespace(s)
