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
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <algorithm>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Dummy
//=============================================================================
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::drawComponent (IDrawContext::Ptr context) {
//    Rectangle bounds = context->clipExtends();
//    context->rect(bounds);
//    context->setStrokeColor(ColorRGBA(0,0,0));
//    context->stroke();
}
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::setForeground(IPattern::Ptr pat) {
    if (!pat) {
        return;
    }
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    svg::graphicElements::Style style = g->getStyleOf(d);
    style.strokePattern(pat);
    g->setStyleTo(d, style);
    svg->redraw();
    Super::setForeground(pat);
}
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::setBackground(IPattern::Ptr pat) {
    if (!pat) {
        return;
    }
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    svg::graphicElements::Style style = g->getStyleOf(d);
    style.fillPattern(pat);
    g->setStyleTo(d, style);
    svg->redraw();
    Super::setBackground(pat);
}
//-----------------------------------------------------------------------------
IPattern::Ptr SvgComponent::Dummy::getForegroundPattern() const {
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    svg::graphicElements::Style style = g->calculateStyle(d);
    return style.strokePattern();
}
//-----------------------------------------------------------------------------
IPattern::Ptr SvgComponent::Dummy::getBackgroundPattern() const {
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    svg::graphicElements::Style style = g->calculateStyle(d);
    return style.fillPattern();
}
//=============================================================================
//  Class SvgComponent
//=============================================================================
//-----------------------------------------------------------------------------
SvgComponent::SvgComponent() {
    setName("SvgComponent");
}
//-----------------------------------------------------------------------------
svg::SvgRootPtr SvgComponent::getSvgObject() const {
    svg::SvgRootPtr root = svgImage->getSvgObject();
    if (!root) {
        throw std::runtime_error("SvgComponent::getSvgObject NULL_PTR");
    }
    return root;
}
//-----------------------------------------------------------------------------
void SvgComponent::postConstructor() {
    setLayout(ALayoutManager::Ptr());
}
//-----------------------------------------------------------------------------
void SvgComponent::drawComponent (IDrawContext::Ptr context) {
    svgImage->draw(context);
}
//-----------------------------------------------------------------------------
void SvgComponent::setupSvgObject(svg::SvgRoot::Ptr obj) {
    removeAll();
    elMap.clear();
    updateDummies();
    setPreferredSize(obj->getSize().getDimension());
}
//-----------------------------------------------------------------------------
void SvgComponent::setSvgString(const std::string &str) {
    svgImage = svg::Image::create();
    svgImage->setSvgString(str);
    setupSvgObject(getSvgObject());
}
//-----------------------------------------------------------------------------
void SvgComponent::setSvgFilename(const std::string &name) {
    svgImage = svg::Image::create();
    svgImage->setSvgPath(name);
    setupSvgObject(getSvgObject());
}
//-----------------------------------------------------------------------------
SvgComponent::DummyPtr SvgComponent::getDummyById(const std::string &id) {
    svg::graphicElements::SceneGraph::Ptr g = getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr x = g->getElementById(id);
    return getDummy(x);
}
//-----------------------------------------------------------------------------
void SvgComponent::getDummiesByClass(const std::string &_class, std::vector<DummyPtr> &out)
{
    svg::graphicElements::SceneGraph::Ptr g = getSvgObject()->getRelatedSceneGraph();
    std::vector<IDrawable::Ptr> objects;
    g->getElementsByClass(_class, objects);
    BOOST_FOREACH(IDrawable::Ptr x, objects) {
        if (!x) {
            continue;
        }
	DummyPtr d = getDummy(x);
	if (d) {
	    out.push_back(d);
	}
    }
}
//-----------------------------------------------------------------------------
SvgComponent::DummyPtr SvgComponent::getDummyOrCreateNew(IDrawable::Ptr x) {
    if (!x) {
        return SvgComponent::DummyPtr();
    }
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
SvgComponent::DummyPtr SvgComponent::getDummy(IDrawable::Ptr x) {
    if (!x) {
        return DummyPtr();
    }
    ElementMap::iterator it = elMap.find(x);
    if (it==elMap.end()) {
        return DummyPtr();
    }
    DummyPtr res = it->second.lock();
    return res;
}
//-----------------------------------------------------------------------------
IDrawable::Ptr SvgComponent::getDrawable(DummyPtr x) {
    if (!x) {
	return IDrawable::Ptr();
    }
    boost::weak_ptr<IDrawable> res;
    x->getClientProperty("svg.element", res);
    return res.lock();
}
//-----------------------------------------------------------------------------
SvgComponent::DummyPtr SvgComponent::createDummy(IDrawable::Ptr x) {
    svg::graphicElements::SceneGraph::Ptr g = getSvgObject()->getRelatedSceneGraph();
    DummyPtr res = Dummy::create();
    res->putClientProperty("svg.element", boost::weak_ptr<IDrawable>(x));
    res->drawable = x;
    std::stringstream ss;
    ss<<"<"<<g->getTagName(x)<<" id='"<<g->getIdName(x)<<"' ";
    ss<<"class='";
    std::vector<std::string> classes;
    g->getClassNames(x, classes);
    BOOST_FOREACH(const std::string &x, classes) {
        ss<<x<<" ";
    }
    ss<<"'/>";
    res->setName(ss.str());
    return res;
}
//-----------------------------------------------------------------------------
void SvgComponent::updateDummies() {
    std::vector<IDrawable::Ptr> elements;
    svg::graphicElements::SceneGraph::Ptr g = getSvgObject()->getRelatedSceneGraph();
    getGraphElementsBySelector(".disco", g, elements);
    BOOST_FOREACH(IDrawable::Ptr x, elements) {
        SvgComponent::DummyPtr dummy = getDummyOrCreateNew(x);
        dummy->setBounds(g->getBoundingBox(x));
    }
}
//-----------------------------------------------------------------------------
void SvgComponent::setStretchToFit(bool stretch) {
    stretchToFit = stretch;
}
//-----------------------------------------------------------------------------
void SvgComponent::doLayout() {
    svgImage->setSize(getSize(), stretchToFit);
    updateDummies();
    updateDrawOrder();
}
//------------------------------------------------------------------------------
void SvgComponent::updateDrawOrder() {
    // update z-order
    using namespace sambag::disco::svg::graphicElements;
    SceneGraph::Ptr sg = getSvgObject()->getRelatedSceneGraph();
    int c=0;
    BOOST_FOREACH(IProcessListObject::Ptr x, sg->getProcessList()) {
        ProcessDrawable::Ptr pd = boost::dynamic_pointer_cast<ProcessDrawable>(x);
        if (!pd) {
            continue;
        }
        IDrawable::Ptr d = pd->drawable;
        DummyPtr dummy = getDummy(d);
        if (dummy) {
            dummy->putClientProperty("svg.z", c++);
        }
    }
}
//------------------------------------------------------------------------------
AComponentPtr SvgComponent::findComponentAt (const Point2D &p, bool includeSelf) 
{
    AComponent::Ptr res;
    BOOST_FOREACH(AComponent::Ptr x, getComponents()) {
	const Rectangle &a = x->getBounds();
	if (a.contains(p)) {
	    if (!res) {
		res=x;
		continue;
	    }
	    int zCurr=INT_MAX, z=INT_MAX;
	    res->getClientProperty("svg.z", zCurr);
	    x->getClientProperty("svg.z", z);
	    if (z<zCurr) {
		res=x;
	    }
	}
    }
    if(!res) {
	if (includeSelf) {
	    return getPtr();
	}
    }
    return res;
}
}}} // namespace(s)
