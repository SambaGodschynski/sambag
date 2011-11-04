/*
 * RefElement.cpp
 *
 *  Created on: 07.10.2011
 *      Author: samba
 */

#include "RefElement.hpp"
#include <boost/graph/breadth_first_search.hpp>
#include <list>
#include <boost/unordered_map.hpp>
#include "assert.h"

namespace {
using namespace boost;
using namespace sambag;
using namespace sambag::disco::graphicElements;

//=============================================================================
class BFSVisitor : public bfs_visitor<> {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	typedef SceneGraph::Vertex Vertex;
	//-------------------------------------------------------------------------
	disco::IDrawable::Ptr currentParent;
	//-------------------------------------------------------------------------
	SceneGraph::Ptr sceneGraph;
	//-------------------------------------------------------------------------
	// mapping from src to created ref
	typedef boost::unordered_map<disco::IDrawable::Ptr, disco::IDrawable::Ptr>
		Src2Dst;
	//-------------------------------------------------------------------------
	Src2Dst old2New;
public:
	//-------------------------------------------------------------------------
	BFSVisitor(SceneGraph::Ptr sceneGraph,
	const Vertex &startVertex,
	disco::IDrawable::Ptr startParent)
	: sceneGraph(sceneGraph)
	{
		disco::IDrawable::Ptr start = sceneGraph->getSceneGraphElement(startVertex);
		old2New[start] = startParent;
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void examine_vertex(const Vertex &u, Graph &g) {
		disco::IDrawable::Ptr old = sceneGraph->getSceneGraphElement(u);
		Src2Dst::iterator it = old2New.find(old);
		assert(it!=old2New.end());
		currentParent = it->second;
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_vertex(const Vertex &u, Graph &g) {
		if (!currentParent)
			return;
		// create ref
		GraphicElement::Ptr old =
			shared_dynamic_cast<GraphicElement>
				(sceneGraph->getSceneGraphElement(u));
		if (!old)
			return;
		RefElement::Ptr neu = RefElement::create();
		neu->setReference(old);
		old2New[old] = neu;
		// append ref
		sceneGraph->addElement(neu);
		sceneGraph->connectElements(currentParent, neu);
		// style
		sceneGraph->adoptStyleEdges(old, neu);
		// transformation
		sceneGraph->adoptTransformEdges(old, neu);
	}
};

}
namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// class RefElement
//=============================================================================
//-----------------------------------------------------------------------------
void RefElement::completeReferenceGraph( SceneGraph::Ptr g ) {
	if (!ref)
		return;
	IDrawable::Ptr self = getPtr();
	// update style
	Style nSt = g->getStyleOf(ref);
	nSt.add(g->getStyleOf(self));
	g->setStyleTo(self, nSt);
	// update transformation
	Matrix nM = g->getTransformationOf(ref);
	boost::numeric::ublas::prod(nM, g->getTransformationOf(self));
	//g->setTransfomationTo(self, nM);
	// walk through
	SceneGraph::Vertex v = g->getRelatedVertex(ref);
	BFSVisitor bfsVis(g, v, self);
	boost::breadth_first_search(g->getGraphImpl(), v, boost::visitor(bfsVis));
}

}}} // namespace
