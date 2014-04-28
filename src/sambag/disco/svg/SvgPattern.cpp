/*
 * SvgPattern.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: samba
 */

#include "SvgPattern.hpp"
#include "sambag/disco/IDiscoFactory.hpp"
namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgPattern
//=============================================================================
//-----------------------------------------------------------------------------
IPattern::Ptr SvgPattern::createPattern() const {
    /*   root->initCreatedObjects();
    ISurface::Ptr sf = getDiscoFactory()->
	createImageSurface(100,100);//size.width().solve(100), size.height().solve(100));
    IDrawContext::Ptr cn = getDiscoFactory()->createContext(sf);
    root->getRelatedSceneGraph()->draw(cn);
    IPattern::Ptr res = getDiscoFactory()->createSurfacePattern(sf);
    res->setExtendType(IPattern::DISCO_EXTEND_REPEAT);
    return res;*/
    return IPattern::Ptr();
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
}
}}}
