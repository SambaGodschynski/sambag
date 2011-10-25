/*
 * SceneGraph.cpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#include "GraphicElement.hpp"
#include "SceneGraph.hpp"
#include "sambag/com/Common.hpp"
#include <sstream>
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
	if (resetContextState==true) {
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
std::string SceneGraph::processListAsString() const {
	typedef std::list<IProcessListObject::Ptr> L;
	L l;
	getProcessList<L>(l);
	if (l.empty())
		return "{}";
	std::stringstream ss;
	L::const_reverse_iterator it = l.rbegin();
	ss<<(*it++)->toString();
	for ( ;it!=l.rend(); ++it) {
		ss<<", "<<(*it)->toString();
	}
	ss<<"}";
	return ss.str();
}
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
	vertexOrderMap[u] = NO_ORDER_NUMBER;
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
//----------------------------------------------------------------------------
bool SceneGraph::setTransfomationTo(const SceneGraphElement &el, const Matrix &m) {
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return false;
	Vertex tv = boost::add_vertex(g);
	vertexTransformationMap[tv] = m;
	vertexTypeMap[tv] = TRANSFORM;
	Edge e; bool succeed;
	boost::tie(e, succeed) = boost::add_edge(tv, rv,g);
	return succeed;
}
//----------------------------------------------------------------------------
bool SceneGraph::setStyleTo(
	const SceneGraphElement &el,
	const graphicElements::Style &s)
{
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return false;
	Vertex tv = boost::add_vertex(g);
	vertexStyleMap[tv] = s;
	vertexTypeMap[tv] = STYLE;
	Edge e; bool succeed;
	boost::tie(e, succeed) = boost::add_edge(tv, rv,g);
	return succeed;
}
//----------------------------------------------------------------------------
const Matrix &
SceneGraph::getTransformationOf(const SceneGraphElement &el) const
{
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return NULL_MATRIX;
	// find parent transformation node(s)
	typedef std::list<Vertex> Vertices;
	Vertices vertices;
	findParentsWithType(rv, TRANSFORM, vertices);
	if (vertices.empty())
		return NULL_MATRIX;
	// assume only one style node per vertex
	return vertexTransformationMap[vertices.back()];
}
//----------------------------------------------------------------------------
const graphicElements::Style &
SceneGraph::getStyleOf(const SceneGraphElement &el) const
{
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return graphicElements::NULL_STYLE;
	// find parent style node(s)
	typedef std::list<Vertex> Vertices;
	Vertices vertices;
	findParentsWithType(rv, STYLE, vertices);
	if (vertices.empty())
		return graphicElements::NULL_STYLE;
	// assume only one style node per vertex
	return vertexStyleMap[vertices.back()];
}
//-----------------------------------------------------------------------------
void SceneGraph::draw(IDrawContext::Ptr context) const {
	typedef std::list<IProcessListObject::Ptr> Elements;
	Elements elements;
	getProcessList<Elements>(elements);
	reverse_for_each( IProcessListObject::Ptr o, elements ) {
		o->perform(context);
	}
}
}}} // namespaces
