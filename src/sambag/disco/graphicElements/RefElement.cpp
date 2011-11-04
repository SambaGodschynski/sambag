/*
 * RefElement.cpp
 *
 *  Created on: 07.10.2011
 *      Author: samba
 */

#include "RefElement.hpp"


namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// class RefElement
//=============================================================================
//-----------------------------------------------------------------------------
void RefElement::setReference(GraphicElement::Ptr r, SceneGraph::Ptr g) {
	// make connection to ref element
	ref = r;
	// TODO: remove probably existing connections
	g->connectElements(__self.lock(), ref);
}

}}} // namespace
