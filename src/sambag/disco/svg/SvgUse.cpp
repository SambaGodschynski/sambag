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
	SvgObject::Ptr svg = root->getObjectById(href);
	if (!svg) return;
	object->setReference(svg->getGraphicElement()->clone());
	Style style = object->getStyle();
	object->getReference()->copyStyleFrom(style);
}
}}}
