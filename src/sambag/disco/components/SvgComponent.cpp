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

namespace sambag { namespace disco { namespace components {
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
void SvgComponent::doLayout() {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    Dimension s = rootObject->getSize().getDimension();
    if (getHeight()==0 || getWidth() == 0) {
        return;
    }
    math::Matrix m = math::scale2D(getWidth()/s.width(), getHeight()/s.height());
    g->setTransfomationTo(g->getRootElement(), m);
}
}}} // namespace(s)
