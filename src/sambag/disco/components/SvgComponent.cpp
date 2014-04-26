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
    Rectangle bounds = context->clipExtends();
    context->rect(bounds);
    context->setStrokeColor(ColorRGBA(0,0,0));
    context->stroke();
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
    createComponentTree();
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
SvgComponent::DummyPtr SvgComponent::getDummyById(const std::string &id) {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    IDrawable::Ptr x = g->getElementById(id);
    return getDummy(x);
}
//-----------------------------------------------------------------------------
void SvgComponent::getDummiesByClass(const std::string &_class, std::vector<DummyPtr> &out)
{
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    std::vector<IDrawable::Ptr> objects;
    g->getElementsByClass(_class, objects);
    BOOST_FOREACH(IDrawable::Ptr x, objects) {
        if (!x) {
            continue;
        }
        out.push_back(getDummy(x));
    }
}
//-----------------------------------------------------------------------------
SvgComponent::DummyPtr SvgComponent::getDummy(IDrawable::Ptr x) {
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
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
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
void SvgComponent::updateDummyBounds(AContainer::Ptr c) {
    if (!c) {
	return;
    }
    std::cout<<c->getName()<<std::endl;
    svg::graphicElements::SceneGraph::Ptr sg = 
	rootObject->getRelatedSceneGraph();
    BOOST_FOREACH(AComponent::Ptr x, c->getComponents()) {
	Dummy::Ptr dummy = boost::dynamic_pointer_cast<Dummy>(x);
	if (!dummy) {
	    continue;
	}
 
	// get absolute (svg) bounds
	IDrawable::Ptr el = getDrawable(dummy);
	Rectangle b = sg->getBoundingBox(el);
	if (b==NULL_RECTANGLE) {
	    continue;
	}
	// make relative to parent
	Point2D loc = b.x0();
	boost::geometry::subtract_point(loc, c->getLocation());
	b.x0(loc);
	dummy->setBounds(b);
	std::cout<<"    "<<dummy->getName()<<" "<<dummy->getBounds()<<std::endl;
	updateDummyBounds(dummy);
    }
}
//-----------------------------------------------------------------------------
void SvgComponent::updateDummyBounds() {
    std::cout<<"----------------------"<<std::endl;
    updateDummyBounds(getPtr());
}
//------------------------------------------------------------------------------
void SvgComponent::createComponentTree() {
    typedef std::vector<IDrawable::Ptr> Elements;
    Elements elements;
    // get all emements of .disco
    svg::graphicElements::SceneGraph::Ptr sg = rootObject->getRelatedSceneGraph();
    getGraphElementsBySelector(".disco", sg, elements);
    typedef svg::graphicElements::SceneGraph::Vertex Vertex;
    typedef svg::graphicElements::SceneGraph::G G;
    const G &g = sg->getGraphImpl();
    // perform breadth search
    typedef std::vector<Vertex> Vertices;
    Vertices p(boost::num_vertices(g));
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

    BOOST_FOREACH(IDrawable::Ptr x, elements) {
	// find disco parent
	Vertex it = sg->getRelatedVertex(x);
	AContainer::Ptr parent;
	while(true) { // find .disco parent
	    it = p.at(it);
	    if (it==end) {
		parent = getPtr();
		break;
	    }
	    Elements::const_iterator pEl = 
		std::find(elements.begin(), 
			  elements.end(), 
			  sg->getSceneGraphElement(it));
	    if(pEl!=elements.end()) {
		// found a disco parent
		parent = getDummy(*pEl);
		break;
	    }
	}
	SAMBAG_ASSERT(parent);
	SvgComponent::DummyPtr dummy = getDummy(x);
	parent->add(dummy);
    }		
}

//-----------------------------------------------------------------------------
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
    updateDummyBounds();
}
}}} // namespace(s)
