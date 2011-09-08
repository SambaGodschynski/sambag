/*
 * SceneGraph.cpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#include "GraphicElement.hpp"
#include "SceneGraph.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// class SceneGraph
//=============================================================================
//-----------------------------------------------------------------------------
bool SceneGraph::addElement( GraphicElementPtr ptr ) {
	bool inserted;
	Element2Vertex::iterator it;
	tie(it, inserted) = element2Vertex.insert(std::make_pair(ptr, Vertex()));
	if (!inserted)
		return false;
	const Vertex &u = add_vertex(g);
	vertexElementMap[u] = ptr;
	it->second = u;
	return true;
}
//-----------------------------------------------------------------------------
bool SceneGraph::connectElements(GraphicElementPtr from, GraphicElementPtr to) {
	Element2Vertex::iterator it;
	it = element2Vertex.find(from);
	// find "from" vertex
	if (it==element2Vertex.end())
		return false;
	Vertex vFrom = it->second;
	// find "to" vertex
	it = element2Vertex.find(to);
	if (it==element2Vertex.end())
		return false;
	Vertex vTo = it->second;
	Edge e;
	bool connected;
	tie(e, connected) = add_edge(vFrom, vTo, g);
	return connected;
}
//-----------------------------------------------------------------------------
const SceneGraph::SceneGraphElement &
SceneGraph::getSceneGraphElement( const SceneGraph::Vertex &v ) const
{
	return vertexElementMap[v];
}
//-----------------------------------------------------------------------------
void SceneGraph::draw(IDrawContext::Ptr context) const {
	typedef std::list<SceneGraphElement> Elements;
	Elements elements;
	getElementsSorted<Elements>(elements);
	for_each( SceneGraphElement o, elements ) {
		o->draw(context);
	}
}
}}} // namespaces
