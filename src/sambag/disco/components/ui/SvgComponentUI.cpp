#include "SvgComponentUI.hpp"
#include <sambag/disco/IDrawable.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <exception>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
// class SvgComponentUIBase
//=============================================================================
//-----------------------------------------------------------------------------
SvgComponent::Ptr SvgComponentUIBase::getSvgComponent(AComponent::Ptr c) {
    SvgComponent::Ptr svg = c->getFirstContainer<SvgComponent>();
    if (!svg) {
        throw std::runtime_error("SvgComponentUIBase no svg parent found");
    };
    return svg;
}
//-----------------------------------------------------------------------------
svg::graphicElements::ISceneGraph::Ptr
SvgComponentUIBase::getSceneGraph(SvgComponent::Ptr svg)
{

    svg::graphicElements::ISceneGraph::Ptr g = svg->getSceneGraph();
    if (!g) {
        throw std::runtime_error("no related scenegraph");
    };
    return g;
}
//-----------------------------------------------------------------------------
SvgComponent::Dummy::Ptr
SvgComponentUIBase::getFirstChildOfClass(const std::string &className,
    SvgComponent::Dummy::Ptr c,
    SvgComponent::Ptr svg,
    svg::graphicElements::ISceneGraph::Ptr g)
{
    if (!svg) {
        svg = getSvgComponent(c);
    }
    if (!g) {
        g = getSceneGraph(svg);
    }
    IDrawable::Ptr x = svg->getDrawable(c);
    if (!x) {
        throw std::runtime_error("no related drawable for '" + c->getName() + "'");
    };
    svg::graphicElements::ISceneGraph::Elements l;
    g->getChildrenByClassX(x, className, l);
    if (l.empty()) {
        throw std::runtime_error("SvgComponentUI: '" + className + "' not found");
    }
    x = *(l.begin());
    SvgComponent::Dummy::Ptr res = svg->getDummy(x);
    if (!res) {
        throw std::runtime_error("no related dummy for '" + x->toString() + "'");
    };
    return res;
}
}}}}