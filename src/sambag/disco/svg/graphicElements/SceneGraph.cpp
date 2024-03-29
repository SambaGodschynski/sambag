/*
 * SceneGraph.cpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#include "GraphicElement.hpp"
#include "SceneGraph.hpp"
#include "SceneGraphHelper.hpp"
#include "sambag/com/Common.hpp"
#include <sstream>
#include <limits.h>
#include <limits>
#include <sambag/disco/Shape.hpp>
#include <sambag/math/Matrix.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include "Compound.hpp"
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {

namespace {
//=============================================================================
/**
 * @class performs a iDrawObject->draw()
 */
struct ProcessDrawable : public IProcessListObject {
//=============================================================================
	//-------------------------------------------------------------------------
	virtual ~ProcessDrawable() {}
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<ProcessDrawable> Ptr;
    //-------------------------------------------------------------------------
    IDrawable::Ptr drawable;
    //-------------------------------------------------------------------------
    // reset context state after draw. otherwise a RestoreContextState
    // will do it later.
    const bool resetContextState;
    //-------------------------------------------------------------------------
    boost::shared_ptr<Style> style;
    //-------------------------------------------------------------------------
    IPattern::Ptr fill, stroke;
    //-------------------------------------------------------------------------
    boost::shared_ptr<Matrix> transformation;
    //-------------------------------------------------------------------------
    ProcessDrawable(IDrawable::Ptr drawable,
		    bool resetContextState,
		    boost::shared_ptr<Style> style,
		    boost::shared_ptr<Matrix> transformation) :
        drawable(drawable),
        resetContextState(resetContextState),
        style(style),
        transformation(transformation)
    {
    }
    //-------------------------------------------------------------------------
    IDrawable::Ptr getDrawable() const {
        return drawable;
    }
    //-------------------------------------------------------------------------
    virtual std::string toString() const {
	return (resetContextState ? "DrawAndRestoreContextState(" : "Draw(")
	    + drawable->toString() + ")";
    }
    //-------------------------------------------------------------------------
    void drawShape(Shape::Ptr shape, IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    void drawShapeImpl(Shape::Ptr shape, IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual void perform(IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual void skip(IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual Rectangle getBounds(IDrawContext::Ptr context) const;
};
} // namespace
///////////////////////////////////////////////////////////////////////////////
IProcessListObject::Ptr createProcessDrawable( IDrawable::Ptr drawable,
    bool resetContextState, boost::shared_ptr<Style> style,
        boost::shared_ptr<Matrix> transformation)
{
    ProcessDrawable::Ptr neu( new ProcessDrawable(drawable,
        resetContextState, style, transformation)
    );
    return neu;
}

//=============================================================================
// class ProcessDrawable
//=============================================================================
//-----------------------------------------------------------------------------
void ProcessDrawable::drawShape(Shape::Ptr shape, IDrawContext::Ptr context) {
    namespace ublas=boost::numeric::ublas;
    using namespace sambag::math;
    bool isFilled = context->isFilled();
    bool isStroked = context->isStroked();
    IPattern::Ptr fpat, spat;
    if (style) {
        fpat = style->fillPattern();
        spat = style->strokePattern();
    }
    if (isFilled || isStroked) {
        shape->shape(context);
    }
    if (isFilled) {
        if (fpat) {
            if (fill!=fpat) { //prepare pattern (once)
                Rectangle b = context->pathExtends();
                // pattern matrices: inverse values, inverse mul order!
                math::Matrix matr = fpat->getMatrix();
                Rectangle patBox = fpat->getBounds();
                if (patBox!=NULL_RECTANGLE) 
                {
                    Number w = patBox.width()>0 ? (Number)patBox.width()  : 1.;
                    Number h = patBox.height()>0? (Number)patBox.height() : 1.;
                    matr = ublas::prod(matr, scale2D(w/b.width(), h/b.height()));
                }
                matr = ublas::prod(matr, translate2D(-b.x(), -b.y()));
                fpat->setMatrix(matr);
                fill=fpat;
            }
            context->setFillPattern(fill);
        }
        if (isStroked) {
            context->fillPreserve();
        } else {
            context->fill();
        }
    }
    if (isStroked) {
        if (spat) {
            if(stroke!=spat) { // prepare stroke pattern (once)
                Rectangle b = context->pathExtends();
                // pattern matrices: inverse values, inverse mul order!!
                math::Matrix matr = spat->getMatrix();
                Rectangle patBox = spat->getBounds();
            if (patBox!=NULL_RECTANGLE && patBox.width()!=0 &&
                patBox.height()!=0) 
                {
                    Number w = patBox.width();
                    Number h = patBox.height();
                    matr = ublas::prod(matr, scale2D(w/b.width(), h/b.height()));
                }
                matr = ublas::prod(matr, translate2D(-b.x(), -b.y()));
                spat->setMatrix(matr);
                stroke = spat;
            }
            context->setStrokePattern(stroke);
        }
        context->stroke();
    }
}
//-----------------------------------------------------------------------------
void ProcessDrawable::perform(IDrawContext::Ptr context) {
    context->save();
    if (transformation) {
        context->transform( *(transformation.get()) );
    }
    if (style) {
        style->intoContext(context);
    }
    
    Shape::Ptr shape = boost::dynamic_pointer_cast<Shape>(drawable);
    if (!shape) {
        drawable->draw(context);
    } else {
        drawShape(shape, context);
    }
    
    // only need to restore state if no children in scenegraph.
    // otherwise state will be restored later with RestoreContextState.
    if (resetContextState==true) {
        context->restore();
    }
};
//-----------------------------------------------------------------------------
void ProcessDrawable::skip(IDrawContext::Ptr context) {
    namespace ublas=boost::numeric::ublas;
    using namespace sambag::math;
    if (!resetContextState) {
        // we have to keep the save/restore order
        context->save();
    }
};
//-----------------------------------------------------------------------------
Rectangle ProcessDrawable::getBounds(IDrawContext::Ptr context) const {
    namespace ublas=boost::numeric::ublas;
    using namespace sambag::math;
    Rectangle res = context->clipExtends();
    if (style) {
        style->intoContext(context);
    }
    {
        /*
            cairo's path_extends returns user space values.
            so when we append all the transformation the resulting
            bounds will not fit (especially when rotation is involved).
            A workaround would be to draw the path without any
            transformation and convert the resulting rect
            via userToDevice(). One problem still exists:
            When rotating a circle the resulting rect will be roatated
            which increases the actual bounding area.
        */
        IDrawContext::Ptr context = getDiscoFactory()->createContext();
        context->rect(res);
        context->clip();
        if (style) {
            style->intoContext(context);
        }
        Shape::Ptr shape = boost::dynamic_pointer_cast<Shape>(drawable);
        if (!shape) {
            Rectangle b = drawable->getBoundingBox(context);
            context->setStrokeColor(ColorRGBA(0,0,0));
            context->setStrokeWidth(1.0);
            context->rect(b);
        } else {
            shape->shape(context);
        }
        res = context->pathExtends();
    
        context->stroke();
    }
    res.inset(context->getStrokeWidth(), context->getStrokeWidth());
    context->save();
    if (transformation) {
        context->transform( *(transformation.get()) );
    }
    Point2D lt = res.x0();
    Point2D lb(res.x0().x(), res.x1().y());
    Point2D rb=res.x1();
    Point2D rt(res.x1().x(), res.x0().y());
    context->userToDevice(lt);
    context->userToDevice(lb);
    context->userToDevice(rb);
    context->userToDevice(rt);
    Point2D p0 = minimize(lt, lb);
    p0 = minimize(p0, rb);
    p0 = minimize(p0, rt);
    Point2D p1 = maximize(lt, lb);
    p1 = maximize(p1, rb);
    p1 = maximize(p1, rt);
    res=Rectangle(p0,p1);
    
    if (resetContextState==true) {
		context->restore();
	}
    return res;
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
std::string SceneGraph::processListAsString() {
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
    if (!inserted) {
        return false;
    }
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
    if (it==element2Vertex.end()) {
        return false;
    }
    Vertex vFrom = it->second;
    // find "to" vertex
    it = element2Vertex.find(to);
    if (it==element2Vertex.end()) {
        return false;
    }
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
Style SceneGraph::calculateStyle(SceneGraphElement el) {
    Vertex v = getRelatedVertex(el);
    if (v==NULL_VERTEX) {
        return Style();
    }
    // perform breadth search
    std::vector<Vertex> p(boost::num_vertices(g));
    Vertex s = *(boost::vertices(g).first);
    Vertex end = INT_MAX;
    p[s] = end;
    boost::breadth_first_search(g, s,
        boost::visitor(
            boost::make_bfs_visitor(
                boost::record_predecessors(&p[0], boost::on_tree_edge())
            )
        )
    );
    // collect styles
    std::list<StylePtr> styles;
    Vertex it = v;
    while(it!=end) {
        StylePtr style = getStyleRef(it);
        if (style) {
            styles.push_back(style);
        }
        it = p.at(it); // get predecessor
    };
    // add styles
    Style res;
    while(!styles.empty()) {
        res.add(*(styles.front()));
        styles.pop_front();
    }
    return res;
}
//-----------------------------------------------------------------------------
Matrix SceneGraph::calculateTransformation(SceneGraphElement el) {
    Vertex v = getRelatedVertex(el);
    if (v==NULL_VERTEX) {
        return NULL_MATRIX;
    }
    // perform breadth search
    std::vector<Vertex> p(boost::num_vertices(g));
    Vertex s = *(boost::vertices(g).first);
    Vertex end = INT_MAX;
    p[s] = end;
    boost::breadth_first_search(g, s,
        boost::visitor(
            boost::make_bfs_visitor(
                boost::record_predecessors(&p[0], boost::on_tree_edge())
            )
        )
    );
    // collect matrices
    std::list<MatrixPtr> matrices;
    Vertex it = v;
    while(it!=end) {
        MatrixPtr matrix = getTransformationRef(it);
        if (matrix) {
            matrices.push_back(matrix);
        }
        it = p.at(it); // get predecessor
    };
    // compute matrices
    Matrix res = IDENTITY_MATRIX;
    while(!matrices.empty()) {
	res = boost::numeric::ublas::prod(res, *(matrices.front()));
        matrices.pop_front();
    }
    return res;
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
    // scene tree is invalid now
	invalidate();
    return succeed;
}
//----------------------------------------------------------------------------
bool SceneGraph::setStyleTo(
	SceneGraphElement el,
	const Style &s)
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
    // scene tree is invalid now
    invalidate();
	return succeed;
}
//----------------------------------------------------------------------------
SceneGraph::MatrixPtr
SceneGraph::getTransformationRef(SceneGraphElement el) const
{
	Vertex rv = getRelatedVertex(el);
	if (rv==NULL_VERTEX)
		return MatrixPtr();
    return getTransformationRef(rv);
}
//----------------------------------------------------------------------------
SceneGraph::MatrixPtr
SceneGraph::getTransformationRef(Vertex rv) const
{
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
    return getStyleRef(rv);
}
//----------------------------------------------------------------------------
SceneGraph::StylePtr
SceneGraph::getStyleRef(Vertex rv) const
{
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
    IDrawable::Ptr dAble;
	boost_reverse_for_each( IProcessListObject::Ptr o, pl ) {
        dAble = o->getDrawable();
        if (!isVisible(dAble)) {
            o->skip(context);
            continue;
        }
		o->perform(context);
	}
}
//-----------------------------------------------------------------------------
Rectangle SceneGraph::getBoundingBox(SceneGraphElement obj,
	IDrawContext::Ptr cn) const
{
    return getBoundingBox(obj);
}
//-----------------------------------------------------------------------------
Rectangle SceneGraph::getBoundingBox(SceneGraphElement obj) const {
    Element2Bounds::const_iterator it = element2Bounds.find(obj);
    if (it==element2Bounds.end()) {
        return NULL_RECTANGLE;
    }
    return it->second;
}
//-----------------------------------------------------------------------------
Rectangle SceneGraph::computeBoundingBox(IDrawable::Ptr parent) {

    typedef std::numeric_limits<Number> L;
    Rectangle res = Rectangle(
        Point2D(L::max(), L::max()),
        Point2D(-L::max(), -L::max()),
        false
	);
	
    std::vector<SceneGraphElement> l;
    getChildren(parent, l, true);
    boost_for_each(SceneGraphElement x, l) {
        Element2Bounds::const_iterator it = element2Bounds.find(x);
        if (it==element2Bounds.end()) {
            continue;
        }
        Rectangle r = it->second;
        res = Rectangle(
            minimize(res.x0(), r.x0()),
            maximize(res.x1(), r.x1()),
            false
	    );
    }
    return res;
}
//-----------------------------------------------------------------------------
void SceneGraph::validateBounds(const Dimension &size) {
    if (!element2Bounds.empty() && boost::num_vertices(g) > 0) {
        return;
    }
    IDrawContext::Ptr cn = getDiscoFactory()->createContext();
    cn->rect(Rectangle(0,0,size.width(), size.height()));
    cn->clip();
    computeBoundingBoxes(cn);
}
//-----------------------------------------------------------------------------
void SceneGraph::computeBoundingBoxes(IDrawContext::Ptr cn) {
    std::vector<IDrawable::Ptr> parents;
    boost_reverse_for_each( IProcessListObject::Ptr o, processList ) {
        ProcessDrawable::Ptr pr =
            boost::dynamic_pointer_cast<ProcessDrawable>(o);
        if (pr) {
            IDrawable::Ptr x=pr->getDrawable();
            SAMBAG_ASSERT(x);
            Rectangle r = pr->getBounds(cn);
            if(boost::out_degree(getRelatedVertex(x), g)>0) {
                // we have children, so we come back later
                parents.push_back(x);
                typedef std::numeric_limits<Number> L;
                r=Rectangle(Point2D(L::max(), L::max()),
                            Point2D(-L::max(), -L::max()),
                            false);
            }
            element2Bounds[x] = r;
            continue;
        }
        o->perform(cn);
    }
    boost_for_each(IDrawable::Ptr x, parents) {
        element2Bounds[x] = computeBoundingBox(x);
    }
}
//-----------------------------------------------------------------------------
void SceneGraph::setFlag(const Vertex &v, Flags aFlag, bool val) {
    FlagMap::mapped_type &flags = flagMap[v];
    if (val) {
        flags |= (1 << aFlag);
    } else {
        flags &= ~(1 << aFlag);
    }
}
//-----------------------------------------------------------------------------
int SceneGraph::getFlag(const Vertex &v, Flags aFlag) const {
    FlagMap::const_iterator it = flagMap.find(v);
    if (it==flagMap.end()) {
        return -1;
    }
    const FlagMap::mapped_type &flags = it->second;
    FlagMap::mapped_type mask = (1 << aFlag);
    return ((flags & mask) == mask);
}
//-----------------------------------------------------------------------------
void SceneGraph::setFlag(SceneGraphElement el, Flags flag, bool val) {
    setFlag(getRelatedVertex(el), flag, val);
}
//-----------------------------------------------------------------------------
int SceneGraph::getFlag(SceneGraphElement el, Flags flag) const {
    return getFlag(getRelatedVertex(el), flag);
}
//-----------------------------------------------------------------------------
void SceneGraph::setVisible(SceneGraphElement el, bool val) {
    if (!el) {
        return;
    }
    setFlag(el, Invisible, !val);
    std::vector<SceneGraphElement> children;
    getChildren(el, children, true);
    BOOST_FOREACH(SceneGraphElement x, children) {
        setFlag(x, Invisible, !val);
    }
}
//-----------------------------------------------------------------------------
bool SceneGraph::isVisible(SceneGraphElement el) const {
    return getFlag(el, Invisible) != 1;
}
//-----------------------------------------------------------------------------
void SceneGraph::select(const std::string & sel, Elements &c) {
    getGraphElementsBySelector(sel, self.lock(), c);
}
}}}} // namespaces
