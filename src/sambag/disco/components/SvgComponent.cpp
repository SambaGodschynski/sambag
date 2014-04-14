/*
 * SvgComponent.cpp
 *
 *  Created on: Sat Mar 29 15:15:06 2014
 *      Author: Johannes Unger
 */

#include "SvgComponent.hpp"
#include "ui/ALookAndFeel.hpp"
#include <sambag/disco/svg/SvgBuilder.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraphHelper.hpp>
#include <boost/graph/depth_first_search.hpp>

namespace sambag { namespace disco { namespace components {

namespace private_ {
///////////////////////////////////////////////////////////////////////////////
struct DFSVisitor : public boost::dfs_visitor<> {
    //-------------------------------------------------------------------------
    typedef svg::graphicElements::SceneGraph SceneGraph;
    //-------------------------------------------------------------------------
    SceneGraph::Ptr sg;
    //-------------------------------------------------------------------------
    SvgComponent *master;
    //-------------------------------------------------------------------------
    DFSVisitor(const SceneGraph::Ptr sg, SvgComponent *master) :
        sg(sg), master(master) {}
    //-------------------------------------------------------------------------
    template <class Vertex, class Graph>
    void discover_vertex(const Vertex &u, Graph &g) {
        IDrawable::Ptr drw = sg->getSceneGraphElement(u);
        if (!drw) {
            return;
        }
        SvgComponent::DummyPtr dummy = master->getDummy(drw);
        if (!dummy) {
            return;
        }
        std::cout<<"D "<<dummy->toString()<<std::endl;
    }
    //-------------------------------------------------------------------------
    template <class Vertex, class Graph>
    void finish_vertex(const Vertex &u, Graph &g) {
        IDrawable::Ptr drw = sg->getSceneGraphElement(u);
        if (!drw) {
            return;
        }
        SvgComponent::DummyPtr dummy = master->getDummy(drw);
        if (!dummy) {
            return;
        }
        std::cout<<"F "<<dummy->toString()<<std::endl;
    }
};
} // namespace
//=============================================================================
//  Class Dummy
//=============================================================================
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::drawComponent (IDrawContext::Ptr context) {
    Rectangle bounds = context->clipExtends();
    context->rect(bounds);
    context->setStrokeColor(ColorRGBA(0,0,0));
    context->stroke();
}
//=============================================================================
//  Class SvgComponent
//=============================================================================
//-----------------------------------------------------------------------------
SvgComponent::SvgComponent() {
	setName("SvgComponent");
}
//-----------------------------------------------------------------------------
void SvgComponent::postConstructor() {
    setLayout(ALayoutManager::Ptr());
}
//-----------------------------------------------------------------------------
void SvgComponent::drawComponent (IDrawContext::Ptr context) {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
}
//-----------------------------------------------------------------------------
void SvgComponent::setupSvgObject(svg::SvgRoot::Ptr obj) {
    setPreferredSize(obj->getSize().getDimension());
}
//-----------------------------------------------------------------------------
void SvgComponent::setSvgString(const std::string &str) {
    svg::SvgBuilder builder;
    rootObject = boost::dynamic_pointer_cast<svg::SvgRoot>
                (builder.buildSvgFromString(str));
    setupSvgObject(rootObject);
}
//-----------------------------------------------------------------------------
void SvgComponent::setSvgFilename(const std::string &name) {
    svg::SvgBuilder builder;
    rootObject = boost::dynamic_pointer_cast<svg::SvgRoot>
                (builder.buildSvgFromFilename(name));
    setupSvgObject(rootObject);
}
//-----------------------------------------------------------------------------
SvgComponent::DummyPtr SvgComponent::getDummy(IDrawable::Ptr x) {
    ElementMap::iterator it = elMap.find(x);
    if (it==elMap.end()) {
        DummyPtr dummy = createDummy(x);
        add(dummy);
        elMap[x]=dummy;
        return dummy;
    }
    DummyPtr res = it->second.lock();
    return res;
}
//-----------------------------------------------------------------------------
SvgComponent::DummyPtr SvgComponent::createDummy(IDrawable::Ptr x) {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    DummyPtr res = Dummy::create();
    std::stringstream ss;
    ss<<"<"<<g->getTagName(x)<<" id='"<<g->getIdName(x)<<"' ";
//    ss<<"class='";
//    std::vector<std::string> classes;
//    g->getClassNames(x, classes)
//    BOOST_FOREACH(const std::string &x, classes) {
//        ss<<x<<" ";
//    }
//    ss<<"'/>";
    res->setName(ss.str());
    return res;
}
//-----------------------------------------------------------------------------
void SvgComponent::updateDummies() {
    std::vector<IDrawable::Ptr> elements;
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    getGraphElementsBySelector(".Disco", g, elements);
    BOOST_FOREACH(IDrawable::Ptr x, elements) {
        SvgComponent::DummyPtr dummy = getDummy(x);
        dummy->setBounds(g->getBoundingBox(x));
    }
    
    private_::DFSVisitor vis(g, this);
    
    boost::depth_first_search(
		g->getGraphImpl(),
		boost::visitor(vis)
	);
}
//----------------------------------------------------------------------------
void SvgComponent::setStretchToFit(bool stretch) {
    stretchToFit = stretch;
}
//-----------------------------------------------------------------------------
void SvgComponent::doLayout() {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    Dimension s = rootObject->getSize().getDimension();
    if (getHeight()==0 || getWidth() == 0) {
        return;
    }
    math::Matrix m = math::scale2D(getWidth()/s.width(), getHeight()/s.height());
    if (stretchToFit) {
        g->setTransfomationTo(g->getRootElement(), m);
    }
    g->invalidateBounds();
    g->validate(getSize());
    updateDummies();
}
}}} // namespace(s)
