/*
 * Image.cpp
 *
 *  Created on: Fri Jun 27 14:35:04 2014
 *      Author: Johannes Unger
 */

#include "Image.hpp"
#include <sambag/disco/svg/SvgBuilder.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraphHelper.hpp>
#include <algorithm>


namespace {

const std::string SVG_FALLBACK="                                        \
<?xml version=\"1.0\" standalone=\"no\"?>                               \
<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"                        \
\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">                   \
<svg width=\"50\" height=\"50\"                                         \
     xmlns=\"http://www.w3.org/2000/svg\"                               \
     xmlns:xlink=\"http://www.w3.org/1999/xlink\"                       \
     version=\"1.1\">                                                   \
    <rect x='0' y='0' width='100%' height='100%'                        \
	  stroke='none' fill='black'/>                                      \
</svg>                                                                  \
";

} // namespace

namespace sambag { namespace disco { namespace svg {
//=============================================================================
//  Class Image
//=============================================================================
//-----------------------------------------------------------------------------
Image::Image() {
}
//-----------------------------------------------------------------------------
Image::Ptr Image::create() {
    Ptr res(new Image());
    return res;
}
//-----------------------------------------------------------------------------
svg::graphicElements::ISceneGraph::Ptr Image::getSceneGraph() const {
    return getSvgObject()->getRelatedSceneGraph();
}
//-----------------------------------------------------------------------------
void Image::setSvgPath(const std::string &path) {
    svg::SvgBuilder builder;
    try {
        rootObject = boost::dynamic_pointer_cast<svg::SvgRoot>
                    (builder.buildSvgFromFilename(path));
    } catch (...) {
        setSvgString(SVG_FALLBACK);
        throw;
    }
}
//-----------------------------------------------------------------------------
void Image::setSvgString(const std::string &str) {
    svg::SvgBuilder builder;
    rootObject = boost::dynamic_pointer_cast<svg::SvgRoot>
                (builder.buildSvgFromString(str));
}
//-----------------------------------------------------------------------------
void Image::setSize(const Dimension &r, bool stretchToFit) {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
    Dimension s = rootObject->getSize().getDimension();
    if (r.height()==0 || r.width() == 0) {
        return;
    }
    math::Matrix m = math::scale2D(r.width()/s.width(), r.height()/s.height());
    if (stretchToFit) {
        g->setTransfomationTo(g->getRootElement(), m);
    }
    g->invalidateBounds();
    g->validate(r);
}
//-----------------------------------------------------------------------------
void Image::draw(IDrawContext::Ptr context) {
    svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
//    context->rect(Rectangle(rootObject->getSize().getDimension()));
//    context->clip();
	g->draw(context);
}
//-----------------------------------------------------------------------------
Rectangle Image::getBoundingBox(IDrawContext::Ptr context) {
    return rootObject->getSize();
}
//-----------------------------------------------------------------------------
std::string Image::toString() const {
    return "svg.Image";
}
}}} // namespace(s)
