/*
 * SceneGraphHelper.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: samba
 */

#include "SceneGraphHelper.hpp"
#include "RefElement.hpp"

//namespace { occurs boost::concepts::requirement<boost::concepts::failed************ ...
//            on mac osx gcc42
using namespace sambag::disco::svg::graphicElements;
//=============================================================================
/**
 * @class CopyVisitor
 * copies subgraph of SceneGraph into other instance.
 */
class CopyVisitor : public boost::bfs_visitor<> {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	typedef SceneGraph::Vertex Vertex;
	//-------------------------------------------------------------------------
	typedef SceneGraph::SceneGraphElement Element;
	//-------------------------------------------------------------------------
	Element currentParent;
	//-------------------------------------------------------------------------
	SceneGraph::Ptr sceneGraph;
	//-------------------------------------------------------------------------
	SceneGraph::Ptr dst;
	//-------------------------------------------------------------------------
	void copy(const Vertex &v) {
		Element obj = sceneGraph->getSceneGraphElement(v);
		return copy(obj);
	}
	//-------------------------------------------------------------------------
	void copy(Element obj) {
		copyElement(sceneGraph, dst, obj);
	}
public:
	//-------------------------------------------------------------------------
	CopyVisitor(SceneGraph::Ptr srcSceneGraph, SceneGraph::Ptr dstSceneGraph,
	const Vertex &startVertex)
	: sceneGraph(srcSceneGraph), dst(dstSceneGraph)
	{
		SAMBAG_ASSERT(sceneGraph);
		SAMBAG_ASSERT(dst);
		copy(startVertex);
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void examine_vertex(const Vertex &u, Graph &g) {
		Element obj = sceneGraph->getSceneGraphElement(u);
		currentParent = obj;
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_vertex(const Vertex &u, Graph &g) {
		if (!currentParent)
			return;
		// create ref
		Element obj = sceneGraph->getSceneGraphElement(u);
		copy(obj);
		// append ref
		dst->connectElements(currentParent, obj);
	}
};
//}// namespace


namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
// SceneGraph Helper
//=============================================================================
//-----------------------------------------------------------------------------
extern void copyElement(SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	SceneGraph::SceneGraphElement obj)
{
	typedef SceneGraph::Vertex Vertex;
	dst->addElement(obj);
	copyElementTraits(src, dst, obj, obj);
}
//-----------------------------------------------------------------------------
void copyElementTraits(SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	SceneGraph::SceneGraphElement srcEl,
	SceneGraph::SceneGraphElement dstEl
)
{
	typedef SceneGraph::Vertex Vertex;
	dst->setStyleRefTo( dstEl,
		src->getStyleRef(srcEl)
	);
	dst->setTransfomationRefTo( dstEl,
			src->getTransformationRef(srcEl)
	);
	// draw order
	Vertex o = src->getRelatedVertex(srcEl);
	Vertex n = dst->getRelatedVertex(dstEl);
	dst->setVertexOrderNumber(n,
		src->getVertexOrderNumber(o)
	);
	// id
	const std::string &id = src->getIdName(srcEl);
	if (id.length() > 0)
		dst->registerElementId(dstEl, id);
	// classes
	std::list<std::string> classes;
	src->getClassNames(srcEl, classes);
	boost_for_each(const std::string &s, classes) {
		dst->registerElementClass(dstEl, s);
	}
}
//-----------------------------------------------------------------------------
void copySubGraph(SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	const SceneGraph::Vertex &start)
{
	SAMBAG_ASSERT(src!=dst);
	SAMBAG_ASSERT(src && dst);
	CopyVisitor vis(src, dst, start);
	boost::breadth_first_search(src->getGraphImpl(), start, boost::visitor(vis));
}
}}}} // namespace
