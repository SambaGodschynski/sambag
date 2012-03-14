/*
 * SvgUse.cpp
 *
 *  Created on: 06.10.2011
 *      Author: samba
 */

#include "SvgUse.hpp"
#include "SvgRoot.hpp"
#include "sambag/disco/graphicElements/SceneGraphHelper.hpp"

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
	GraphicElement::Ptr ref =
		boost::shared_dynamic_cast<GraphicElement>(getRelatedSceneGraph()->getElementById(href) );
	if (!ref)
		return;
	object->setReference(ref);
	completeReferenceGraph();
}
//-----------------------------------------------------------------------------
void SvgUse::completeReferenceGraph() {
	SceneGraph::Ptr g = getRelatedSceneGraph();
	IDrawable::Ptr ref = object->getReference();
	if (!ref)
		return;
	// update style
	Style nSt = g->getStyleOf(ref);
	nSt.add(g->getStyleOf(object));
	g->setStyleTo(object, nSt);
	// update transformation
	Matrix nM = g->getTransformationOf(ref);
	boost::numeric::ublas::prod(nM, g->getTransformationOf(object));
	//g->setTransfomationTo(self, nM);
	// get subgraph starting with ref
	SceneGraph::Ptr g2 = SceneGraph::create();
	copySubGraph(g, g2, g->getRelatedVertex(ref));
	// copy back
	typedef SceneGraph::SceneGraphElement Element;
	boost::unordered_map<Element, Element> old2New;
	addToGraphAsReference(g2, g, old2New);
	// connect new subgraph
	g->connectElements(object, old2New[g2->getRootElement()]);
}
}}}
