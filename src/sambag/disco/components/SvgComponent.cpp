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
struct SvgComponent::SyncedSceneGraph :
    public sambag::disco::svg::graphicElements::ISceneGraph
{
    typedef boost::shared_ptr<SyncedSceneGraph> Ptr;
    typedef sambag::disco::svg::graphicElements::ISceneGraph Super;
    Super::Ptr g;
    mutable com::RecursiveMutex lock;
    virtual void invalidate()
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->invalidate();
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void invalidateBounds()
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->invalidateBounds();
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void validate(const Dimension &size)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->validate(size);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void validateBounds(const Dimension &size)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->validateBounds(size);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual SceneGraphElement getRootElement()
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getRootElement();
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void draw(IDrawContext::Ptr cn)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->draw(cn);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual bool setTransfomationTo(SceneGraphElement el, const Matrix &m)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->setTransfomationTo(el, m);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual bool setStyleTo(SceneGraphElement el, const svg::graphicElements::Style &s)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->setStyleTo(el, s);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual bool setTransfomationRefTo(SceneGraphElement el, MatrixPtr m)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->setTransfomationRefTo(el, m);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual bool setStyleRefTo(SceneGraphElement el, StylePtr s)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->setStyleRefTo(el, s);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual MatrixPtr getTransformationRef(SceneGraphElement el) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getTransformationRef(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual Matrix getTransformationOf(SceneGraphElement el) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getTransformationOf(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual svg::graphicElements::Style getStyleOf(SceneGraphElement el) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getStyleOf(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual svg::graphicElements::Style calculateStyle(SceneGraphElement el)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->calculateStyle(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual Matrix calculateTransformation(SceneGraphElement el)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->calculateTransformation(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual StylePtr getStyleRef(SceneGraphElement el) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getStyleRef(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void getChildrenX(SceneGraphElement el, Elements &c, bool deep = true ) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getChildrenX(el,c,deep);
        SAMBAG_END_SYNCHRONIZED
    }

    virtual void getChildrenByTagX(SceneGraphElement el,
        const Tag &tagName, Elements &c, bool deep = true) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getChildrenByTagX(el, tagName, c, deep);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void getChildrenByClassX(SceneGraphElement el,
        const Class &className,Elements &c, bool deep = true) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getChildrenByClassX(el, className, c, deep);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void getChildrenByIdX(SceneGraphElement el, const Id &id,
        Elements &c, bool deep = true) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getChildrenByIdX(el, id, c, deep);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void getClassNamesX(SceneGraphElement el, Classes &c) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getClassNamesX(el, c);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void getElementsByClassX(const Class & className, Elements &c)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getElementsByClassX(className, c);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void getElementsByTagX(const Tag & tagName, Elements &c)
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->getElementsByTagX(tagName, c);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual Tag getTagName(SceneGraphElement el) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getTagName(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual Id getIdName(SceneGraphElement el) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getIdName(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual SceneGraphElement getElementById(const Id &id) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getElementById(id);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual Rectangle getBoundingBox(SceneGraphElement obj) const
    {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->getBoundingBox(obj);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void setVisible(SceneGraphElement el, bool val) {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->setVisible(el, val);
        SAMBAG_END_SYNCHRONIZED
    }

    virtual bool isVisible(SceneGraphElement el) const {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->isVisible(el);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual void setDirty(SceneGraphElement el, bool val) {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            g->setDirty(el, val);
        SAMBAG_END_SYNCHRONIZED
    }
    virtual bool isDirty(SceneGraphElement el) const {
        SAMBAG_BEGIN_SYNCHRONIZED(lock)
            return g->isDirty(el);
        SAMBAG_END_SYNCHRONIZED
    }
}; // SceneGraphAdapter

namespace svgExtensions {
    extern void installSvgKnobExtension(SvgComponent::Dummy::Ptr);
    extern void installSvgButtonExtension(SvgComponent::Dummy::Ptr);
}
//=============================================================================
//  Class Dummy
//=============================================================================
//-----------------------------------------------------------------------------
const std::string SvgComponent::Dummy::PROPERTY_MODEL = "property model";
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::drawComponent (IDrawContext::Ptr context) {
//    Rectangle bounds = context->clipExtends();
//    bounds.inset(-2,-2);
//    context->rect(bounds);
//    context->setStrokeColor(ColorRGBA(1,0,0));
//    context->setStrokeWidth(2);
//    context->stroke();
}
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::setForeground(IPattern::Ptr pat) {
    svg::graphicElements::Style style = getStyle();
    style.strokePattern(pat);
    setStyle(style);
}
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::setBackground(IPattern::Ptr pat) {
    svg::graphicElements::Style style = getStyle();
    style.fillPattern(pat);
    setStyle(style);
}
//-----------------------------------------------------------------------------
svg::graphicElements::Style SvgComponent::Dummy::getStyle() const {
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    svg::graphicElements::Style style = g->calculateStyle(d);
    return style;
}
//-----------------------------------------------------------------------------
SvgComponent::Dummy::StylePtr SvgComponent::Dummy::getStyleRef() {
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    return g->getStyleRef(d);
}
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::setStyle(const svg::graphicElements::Style &x) {
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    if (!d) {
        throw std::runtime_error("SvgComponent::Dummy related object == NULL");
    }
    SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
        g->setStyleTo(d, x);
    SAMBAG_END_SYNCHRONIZED
    IPattern::Ptr fill = x.fillPattern();
    if (Super::getBackgroundPattern()!=fill) {
        Super::setBackground(fill);
    }
    IPattern::Ptr stroke = x.strokePattern();
    if (Super::getForegroundPattern()!=stroke) {
        Super::setForeground(stroke);
    }
    svg->redraw();
}
//-----------------------------------------------------------------------------
IPattern::Ptr SvgComponent::Dummy::getForegroundPattern() const {
    svg::graphicElements::Style style = getStyle();
    return style.strokePattern();
}
//-----------------------------------------------------------------------------
IPattern::Ptr SvgComponent::Dummy::getBackgroundPattern() const {
    svg::graphicElements::Style style = getStyle();
    return style.fillPattern();
}
//-----------------------------------------------------------------------------
void SvgComponent::Dummy::setVisible(bool b) {
    if (isVisible()==b) {
        return;
    }
    SvgComponent::Ptr svg = getFirstContainer<SvgComponent>();
    SAMBAG_ASSERT(svg);
    svg::graphicElements::SceneGraph::Ptr g =
        svg->getSvgObject()->getRelatedSceneGraph();
    IDrawable::Ptr d = drawable.lock();
    g->setVisible(d, b);
    svg->redraw();
    Super::setVisible(b);
}
//=============================================================================
//  Class SvgComponent
//=============================================================================
//-----------------------------------------------------------------------------
Dimension SvgComponent::getSvgSize() const {
    return getSvgObject()->getSize().size();
}
//-----------------------------------------------------------------------------
svg::graphicElements::ISceneGraph::Ptr SvgComponent::getSceneGraph() const {
    if (!sceneGraph) {
        sceneGraph = SyncedSceneGraph::Ptr(new SyncedSceneGraph());
        sceneGraph->g = getSvgObject()->getRelatedSceneGraph();
    }
    return sceneGraph;
}
//-----------------------------------------------------------------------------
const SvgComponent::ExtensionRegister &
SvgComponent::getExtensionRegister() const
{
    return exReg;
}
//-----------------------------------------------------------------------------
SvgComponent::ExtensionRegister &
SvgComponent::getExtensionRegister()
{
    return exReg;
}
//-----------------------------------------------------------------------------
SvgComponent::SvgComponent() {
    setName("SvgComponent");
}
//-----------------------------------------------------------------------------
void SvgComponent::initExtendRegister() {
    using namespace boost::assign;
    exReg[".disco-knob"] = &svgExtensions::installSvgKnobExtension;
    exReg[".disco-button"] = &svgExtensions::installSvgButtonExtension;
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
    initExtendRegister();
}
//-----------------------------------------------------------------------------
void SvgComponent::drawComponent (IDrawContext::Ptr context) {
    SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
        svgImage->draw(context);
    SAMBAG_END_SYNCHRONIZED
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
        svg::graphicElements::SceneGraph::Ptr g =
            getSvgObject()->getRelatedSceneGraph();
        std::list<SvgClass> classes;
        g->getClassNames(x, classes);
        installExtension(dummy, classes);
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
void SvgComponent::installExtension(DummyPtr dummy,
    const std::list<SvgClass> &classes)
{
    BOOST_FOREACH(const std::string &x, classes) {
        ExtensionRegister::const_iterator it = exReg.find(x);
        if (it==exReg.end()) {
            continue;
        }
        it->second(dummy);
    }
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
    std::list<SvgClass> classes;
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
        Rectangle r = g->getBoundingBox(x);
        if (r!=NULL_RECTANGLE) {
            dummy->setBounds(r);
        }
    }
}
//-----------------------------------------------------------------------------
void SvgComponent::setStretchToFit(bool stretch) {
    stretchToFit = stretch;
}
//-----------------------------------------------------------------------------
void SvgComponent::doLayout() {
    SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
        svgImage->setSize(getSize(), stretchToFit);
        updateDummies();
        updateDrawOrder();
    SAMBAG_END_SYNCHRONIZED
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
//-----------------------------------------------------------------------------
AComponentPtr SvgComponent::findComponentAt (const Point2D &_p, bool includeSelf)
{
    AComponent::Ptr res;
    BOOST_FOREACH(AComponent::Ptr x, getComponents()) {
        if (!x->isVisible() || !x->isEnabled()) {
            continue;
        }
        Point2D p = _p;
        boost::geometry::subtract_point(p, x->getLocation());
        if (x->contains(p)) {
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
