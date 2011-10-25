/*
 * SceneGraph.cpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#include "GraphicElement.hpp"
#include "SceneGraph.hpp"
#include "sambag/com/Common.hpp"
#include <iostream>
#include <limits.h>

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// class ProcessDrawable
//=============================================================================
//-----------------------------------------------------------------------------
void ProcessDrawable::perform(IDrawContext::Ptr context) {
	// TODO: deprecated code
	GraphicElement::Ptr g = boost::shared_dynamic_cast<GraphicElement>(drawable);
	context->save();
	context->transform( g->getTransformMatrixDeprecated() );
	g->draw(context);
	// only need to restore state if no children in scenegraph.
	// otherwise state will be restored later with RestoreContextState.
	if (numChildren==0) {
		context->restore();
	}

	//std::cout<<"draw: " << typeid(*g.get()).name() << std::endl;

};
//=============================================================================
// class RestoreContextState
//=============================================================================
//-----------------------------------------------------------------------------
void RestoreContextState::perform(IDrawContext::Ptr context) {
	context->restore();
	//std::cout<<"restore: " << name << std::endl;
};
//=============================================================================
const SceneGraph::Vertex SceneGraph::NULL_VERTEX = UINT_MAX;
//=============================================================================
// class SceneGraph
//=============================================================================
//-----------------------------------------------------------------------------
bool SceneGraph::addElement( IDrawable::Ptr ptr ) {
	bool inserted;
	Element2Vertex::iterator it;
	tie(it, inserted) = element2Vertex.insert(std::make_pair(ptr, Vertex()));
	if (!inserted)
		return false;
	const Vertex &u = add_vertex(g);
	vertexElementMap[u] = ptr;
	vertexTypeMap[u] = IDRAWABLE;
	it->second = u;
	return true;
}
//-----------------------------------------------------------------------------
bool SceneGraph::connectElements(IDrawable::Ptr from, IDrawable::Ptr to) {
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
	// set order number
	vertexOrderMap[vTo] = boost::out_degree(vFrom,g);

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
SceneGraph::Vertex SceneGraph::getRelatedVertex(const SceneGraphElement &el) const {
	Element2Vertex::const_iterator it = element2Vertex.find(el);
	if (it==element2Vertex.end())
		return NULL_VERTEX;
	return it->second;
}
//-----------------------------------------------------------------------------
void SceneGraph::draw(IDrawContext::Ptr context) const {
	typedef std::list<ProcessListProcessor::Ptr> Elements;
	Elements elements;
	getProcessList<Elements>(elements);
	reverse_for_each( ProcessListProcessor::Ptr o, elements ) {
		o->perform(context);
	}
}
}}} // namespaces
