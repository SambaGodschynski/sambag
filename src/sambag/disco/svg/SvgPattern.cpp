/*
 * SvgPattern.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: samba
 */

#include "SvgPattern.hpp"
#include "sambag/disco/IDiscoFactory.hpp"
#include "graphicElements/SceneGraphHelper.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgPattern
//=============================================================================
//-----------------------------------------------------------------------------
IPattern::Ptr SvgPattern::createPattern() const {
    //create new graph from defs
    SceneGraph::Ptr g = getRelatedSceneGraph();
    IDrawable::Ptr ref = getGraphicElement();
    SceneGraph::Ptr g2 = SceneGraph::create();
    copySubGraph(g, g2, g->getRelatedVertex(ref));
    // create surface, context, pattern
    ISurface::Ptr sf = getDiscoFactory()->
	createImageSurface(size.width().solve(100), size.height().solve(100));
    IDrawContext::Ptr cn = getDiscoFactory()->createContext(sf);
    g2->draw(cn);
    IPattern::Ptr res = getDiscoFactory()->createSurfacePattern(sf);
    res->setExtendType(IPattern::DISCO_EXTEND_REPEAT);
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
}
}}}
