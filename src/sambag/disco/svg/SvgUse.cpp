/*
 * SvgUse.cpp
 *
 *  Created on: 06.10.2011
 *      Author: samba
 */

#include "SvgUse.hpp"
#include "SvgRoot.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgUse
//=============================================================================
//-----------------------------------------------------------------------------
SvgUse::SvgUse() {
	object = RefElement::create();
}
//-----------------------------------------------------------------------------
void SvgUse::solveHRef() {
	using namespace sambag::disco::graphicElements;
	SvgRoot::Ptr root = boost::shared_dynamic_cast<SvgRoot>(getRoot());
	SvgObject::Ptr refSvg = root->getObjectById(href);
	if (!refSvg) return;
	object->setReference(refSvg->getGraphicElement()->clone());

	// TODO: don't copy style => add a new edge from refStyle to newGEl.
	getRelatedSceneGraph()->setStyleTo(
			object->getReference(),
			getRelatedSceneGraph()->getStyleOf(refSvg->getGraphicElement())
	);
}
}}}
