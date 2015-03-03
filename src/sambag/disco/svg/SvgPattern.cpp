/*
 * SvgPattern.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: samba
 */

#include "SvgPattern.hpp"
#include "sambag/disco/IDiscoFactory.hpp"
#include "graphicElements/SceneGraphHelper.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <sambag/math/Matrix.hpp>

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgPattern
//=============================================================================
//-------------------------------------------------------------------------
SvgPattern::SvgPattern() {
    combo = sambag::disco::svg::graphicElements::Compound::create();
    transform = IDENTITY_MATRIX;
}
//-----------------------------------------------------------------------------
namespace {
    Dimension _calcSize(const Dimension &d, const math::Matrix &m)
    {
	typedef boost::numeric::ublas::vector<double> Vector;
	using namespace sambag::math;
	//if (m==IDENTITY_MATRIX) { //fails to compile: no match for operator ==
	//    return d;
	//}
	// rect centers at origin
	Vector lt(3), rt(3), lb(3), rb(3); 
	lt(0)=-d.width()/2.; lt(1)=-d.height()/2.; lt(2)=0;
	rt(0)= d.width()/2.; rt(1)=-d.height()/2.; rt(2)=0;
	lb(0)=-d.width()/2.; lb(1)= d.height()/2.; lb(2)=0;
	rb(0)= d.width()/2.; rb(1)= d.height()/2.; rb(2)=0;
	// perform transformation
	lt = boost::numeric::ublas::prod(m, lt);
	rt = boost::numeric::ublas::prod(m, rt);
	lb = boost::numeric::ublas::prod(m, lb);
	rb = boost::numeric::ublas::prod(m, rb);
	// create rect
	Point2D p0 = minimize(Point2D(lt(0), lt(1)), Point2D(lb(0),lb(1)));
	p0 = minimize(p0, Point2D(rb(0), rb(1)));
	p0 = minimize(p0, Point2D(rt(0), rt(1)));
	Point2D p1 = maximize(Point2D(lt(0), lt(1)),  Point2D(lb(0), lb(1)));
	p1 = maximize(p1, Point2D(rb(0), rb(1)));
	p1 = maximize(p1, Point2D(rt(0), rt(1)));
	return Rectangle(p0,p1).size();
    }
} 
IPattern::Ptr SvgPattern::createPattern() const {
    //create new graph from defs
    SceneGraph::Ptr g = getRelatedSceneGraph();
    IDrawable::Ptr ref = getGraphicElement();
    SceneGraph::Ptr g2 = SceneGraph::create();
    copySubGraph(g, g2, g->getRelatedVertex(ref));
    // create surface, context, pattern
    Dimension _size(size.width().solve(100), size.height().solve(100));
    //_size=_calcSize(_size, transform);
    ISurface::Ptr sf = getDiscoFactory()->createImageSurface(
	_size.width(), _size.height());
    IDrawContext::Ptr cn = getDiscoFactory()->createContext(sf);
    //cn->transform(transform);
    //cn->translate(Point2D(p.x().solve(100), p.y().solve(100)));
    g2->draw(cn);
    IPattern::Ptr res = getDiscoFactory()->createSurfacePattern(sf);
    res->setExtendType(IPattern::DISCO_EXTEND_REPEAT);
    res->setMatrix(transform);
    return res;
}
//-----------------------------------------------------------------------------
SvgPattern::Ptr SvgPattern::create(SvgRoot *root) {
    Ptr neu(new SvgPattern());
    neu->__setSelf(neu);
    neu->createBase(root);
    //graphicElements::SceneGraph::Ptr g = graphicElements::SceneGraph::create();
    return neu;
}
//-----------------------------------------------------------------------------
void SvgPattern::add(SvgObject::Ptr obj) {
    Super::add(obj);
}
//-----------------------------------------------------------------------------
void SvgPattern::registerAttributes(SvgObject::BuilderType &binder) {
    binder.registerAttribute<X_tag::Type, X_tag, SvgPattern>("x");
    binder.registerAttribute<Y_tag::Type, Y_tag, SvgPattern>("y");
    binder.registerAttribute<Width_tag::Type, Width_tag, SvgPattern>("width");
    binder.registerAttribute<Height_tag::Type, Height_tag, SvgPattern>("height");
    binder.registerAttribute<Transform_tag::Type, Transform_tag, SvgPattern>("patternTransform");
}
}}}
