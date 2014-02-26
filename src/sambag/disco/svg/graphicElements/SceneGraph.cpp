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
#include <limits>
#include <sambag/disco/Shape.hpp>
#include <sambag/math/Matrix.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
// class ProcessDrawable
//=============================================================================
//-----------------------------------------------------------------------------
void ProcessDrawable::perform(IDrawContext::Ptr context) {
	namespace ublas=boost::numeric::ublas;
	using namespace sambag::math;
	context->save();
	if (transformation) {
		context->transform( *(transformation.get()) );
	}
    IPattern::Ptr fpat, spat;
	if (style) {
		style->intoContext(context);
        fpat = style->fillPattern();
		spat = style->strokePattern();
	}
    
    
    Shape::Ptr shape = boost::dynamic_pointer_cast<Shape>(drawable);
	if (!shape) {
        drawable->draw(context);
	} else {
        if (fpat) {
            shape->shape(context);
            Rectangle b = context->pathExtends();
			// pattern matrices: inverse values, inverse mul order!!
			math::Matrix matr = IDENTITY_MATRIX;
            Rectangle patBox = fpat->getBounds();
            if (patBox!=NULL_RECTANGLE) {
      			Number w = patBox.width();
                Number h = patBox.height();
                w = w>0. ? w:1.;
                h = h>0. ? h:1.;
				matr = ublas::prod(matr, scale2D(w/b.width(), h/b.height()));
            }
			matr = ublas::prod(matr, translate2D(-b.x(), -b.y()));
			fpat->setMatrix(matr);
			context->setFillPattern(fpat);
        	context->fill();
        }
		if (spat) {
            shape->shape(context);
            Rectangle b = context->pathExtends();
			// pattern matrices: inverse values, inverse mul order!!
			math::Matrix matr = IDENTITY_MATRIX;
            Rectangle patBox = spat->getBounds();
            if (patBox!=NULL_RECTANGLE) {
      			Number w = patBox.width();
                Number h = patBox.height();
                w = w>0. ? w:1.;
                h = h>0. ? h:1.;
				matr = ublas::prod(matr, scale2D(w/b.width(), h/b.height()));
            }
			matr = ublas::prod(matr, translate2D(-b.x(), -b.y()));
			spat->setMatrix(matr);
			context->setStrokePattern(spat);
        	context->stroke();
        }
    }
    
    // only need to restore state if no children in scenegraph.
	// otherwise state will be restored later with RestoreContextState.
	if (resetContextState==true) {
		context->restore();
	}
};
//=============================================================================
// class RestoreContextState
//=============================================================================
//-----------------------------------------------------------------------------
void RestoreContextState::perform(IDrawContext::Ptr context) {
	context->restore();
};
//=============================================================================
const SceneGraph::Vertex SceneGraph::NULL_VERTEX = UINT_MAX;
//=============================================================================
// class SceneGraph
//=============================================================================
//-----------------------------------------------------------------------------
std::string SceneGraph::processListAsString() const {
	ProcessList l;
	createProcessList(l);
	if (l.empty())
		return "{}";
	std::stringstream ss;
	ProcessList::const_reverse_iterator it = l.rbegin();
	ss<<"{"<<(*it++)->toString();
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
	boost::tie(it, inserted) = element2Vertex.insert(std::make_pair(ptr, Vertex()));
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
bool SceneGraph::registerElementClass(SceneGraphElement el,
		const SceneGraph::Class &className )
{

	Vertex classVertex = NULL_VERTEX;
	Vertex elV = getRelatedVertex(el);
	if (elV == NULL_VERTEX)
		return false;
	Class2Vertex::iterator it = class2Vertex.find(className);
	if (it==class2Vertex.end()) { // classname not yet registered
		// add new class vertex to graph
		classVertex = boost::add_vertex(g);
		vertexTypeMap[classVertex] = CLASS;
		vertexOrderMap[classVertex] = NO_ORDER_NUMBER;
		vertexNameMap[classVertex] = className;
		class2Vertex.insert(std::make_pair(className, classVertex));
	} else {
		classVertex = it->second;
	}
	Edge e; bool exists;
	// edge already exist
	boost::tie(e, exists) = boost::edge(classVertex, elV, g);
	if (exists)
		return false;
	boost::add_edge(classVertex, elV, g);
	return true;
}
//-----------------------------------------------------------------------------
SceneGraph::SceneGraphElement
SceneGraph::getSceneGraphElement( const SceneGraph::Vertex &v ) const
{
	if (getVertexType(v)!=IDRAWABLE)
		return SceneGraphElement();
	return vertexElementMap[v];
}
//-----------------------------------------------------------------------------
size_t SceneGraph::inDegreeOf(const Vertex& v, VertexType type) const {
	size_t result = 0;
	InvAdjacencyIterator it, end;
	boost::tie(it, end) = boost::inv_adjacent_vertices(v,g);
	for(; it!=end; ++it) {
		if (getVertexType(*it) == type)
			++result;
	}
	return result;
}
//-----------------------------------------------------------------------------
SceneGraph::Vertex SceneGraph::getRelatedVertex(SceneGraphElement el) const {
	Element2Vertex::const_iterator it = element2Vertex.find(el);
	if (it==element2Vertex.end())
		return NULL_VERTEX;
	return it->second;
}
//----------------------------------------------------------------------------
IDrawable::Ptr SceneGraph::getElementById(const SceneGraph::Id &id) const {
	Id2Vertex::const_iterator it = id2Vertex.find(id);
	if (it==id2Vertex.end())
		return IDrawable::Ptr();
	return getSceneGraphElement(it->second);
}
//----------------------------------------------------------------------------
bool SceneGraph::setTransfomationTo(SceneGraphElement el, const Matrix &m) {
	return setTransfomationRefTo(el, MatrixPtr(new Matrix(m)));
}
//----------------------------------------------------------------------------
bool SceneGraph::setTransfomationRefTo(SceneGraphElement el, MatrixPtr m) {
	if (!m)
		return false;
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return false;
	std::list<Vertex> vertices;
	findParentsByType(rv, TRANSFORM, vertices);
	if (!vertices.empty()) { // update
		Vertex cur = vertices.back();
		*(vertexTransformationMap[cur]) = *m;
		return true;
	}
	Vertex tv = boost::add_vertex(g);
	vertexTransformationMap[tv] = m;
	vertexTypeMap[tv] = TRANSFORM;
	Edge e; bool succeed;
	boost::tie(e, succeed) = boost::add_edge(tv, rv,g);
	return succeed;
}
//----------------------------------------------------------------------------
bool SceneGraph::setStyleTo(
	SceneGraphElement el,
	const graphicElements::Style &s)
{
	return setStyleRefTo(el, StylePtr(new Style(s)));
}
//----------------------------------------------------------------------------
bool SceneGraph::setStyleRefTo(
	SceneGraphElement el,
	StylePtr s)
{
	if (!s)
		return false;
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return false;
	std::list<Vertex> vertices;
	findParentsByType(rv, STYLE, vertices);
	if (!vertices.empty()) { // update
		Vertex cur = vertices.back();
		*(vertexStyleMap[cur]) = *s;
		return true;
	}
	Vertex tv = boost::add_vertex(g);
	vertexStyleMap[tv] = s;
	vertexTypeMap[tv] = STYLE;
	Edge e; bool succeed;
	boost::tie(e, succeed) = boost::add_edge(tv, rv,g);
	return succeed;
}
//----------------------------------------------------------------------------
SceneGraph::MatrixPtr
SceneGraph::getTransformationRef(SceneGraphElement el) const
{
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return MatrixPtr();
	// find parent transformation node(s)
	typedef std::list<Vertex> Vertices;
	Vertices vertices;
	findParentsByType(rv, TRANSFORM, vertices);
	if (vertices.empty())
		return MatrixPtr();
	// assume only one style node per vertex
	return vertexTransformationMap[vertices.back()];
}
//----------------------------------------------------------------------------
SceneGraph::StylePtr
SceneGraph::getStyleRef(SceneGraphElement el) const
{
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return StylePtr();
	// find parent style node(s)
	typedef std::list<Vertex> Vertices;
	Vertices vertices;
	findParentsByType(rv, STYLE, vertices);
	if (vertices.empty())
		return StylePtr();
	// assume only one style node per vertex
	return vertexStyleMap[vertices.back()];
}
//-----------------------------------------------------------------------------
void SceneGraph::draw(IDrawContext::Ptr context) {
	const ProcessList &pl = getProcessList();
	boost_reverse_for_each( IProcessListObject::Ptr o, pl ) {
		o->perform(context);
	}
}
//-----------------------------------------------------------------------------
Rectangle SceneGraph::getBoundingBox(SceneGraphElement obj,
	IDrawContext::Ptr cn) const
{
	MatrixPtr m = getTransformationRef(obj);
	StylePtr s =  getStyleRef(obj);
	if (m||s) {
		cn->save();
	}
	if (m) {
		cn->transform(*m);
	}
	if (s) {
		s->intoContext(cn);
	}
	Rectangle res = obj->getBoundingBox(cn);
	if (m||s) {
		cn->restore();
	}
	if (res==NULL_RECTANGLE) {
		typedef std::numeric_limits<Number> L;
		res = Rectangle(
			Point2D(L::max(), L::max()),
			Point2D(-L::max(), -L::max()),
			false
		);
	}
	std::list<SceneGraphElement> l;
	getChildren(obj, l, true);
	boost_for_each(SceneGraphElement o, l) {
		MatrixPtr m = getTransformationRef(o);
		StylePtr s =  getStyleRef(o);
		if (m||s) {
			cn->save();
		}	
		if (m) {
			cn->transform(*m);
		}
		if (s) {
			s->intoContext(cn);
		}
		Rectangle r = o->getBoundingBox(cn);		
		if (m||s) {
			cn->restore();
		}
		if (r==NULL_RECTANGLE) {
			continue;
		}
		res = Rectangle(
			minimize(res.x0(), r.x0()),
			maximize(res.x1(), r.x1()),
			false
		);
	}
	return res;
}
}}}} // namespaces
