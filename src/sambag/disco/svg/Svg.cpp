/*
 * SVGGraphicElement.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "Svg.hpp"
#include "SvgDefs.hpp"
#include "sambag/com/Common.hpp"
#include "SvgRoot.hpp"
#include "SvgPattern.hpp"
#include "sambag/com/Exception.hpp"
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgObject
//=============================================================================
//-----------------------------------------------------------------------------
void SvgObject::createBase(SvgRoot* root) {
	if (!root)
		return;
	svgRootObject = root->getPtr();
	setRelatedSceneGraph(root->getRelatedSceneGraph());
}
//-----------------------------------------------------------------------------
void SvgObject::setClassName(const std::string & str) {
    using namespace boost::algorithm;
    std::vector<std::string> classes;
    split(classes, str, is_any_of(" "), token_compress_on);
    BOOST_FOREACH(const std::string &x, classes) {
        sceneGraph->registerElementClass(getGraphicElement(), "." + x);
    }
}
//-----------------------------------------------------------------------------
void SvgObject::add(Ptr obj) {
   	graphicElements::SceneGraph::Ptr g = getRelatedSceneGraph();
	if (!g)
		return;
	if(dynamic_cast<SvgDefs*>(obj.get())) // a def is invisible so we doesn't hook it in.
		return;
	graphicElements::GraphicElement::Ptr gO = obj->getGraphicElement();
	g->connectElements(getGraphicElement(), gO);
}
//-----------------------------------------------------------------------------
void SvgObject::onFillObject(SvgObject::Ptr fillObj) {
	SvgPatternBase::Ptr svgPattern = boost::dynamic_pointer_cast<SvgPatternBase>(fillObj);
	if (!svgPattern)
		return;
	GraphicElement::Ptr obj = getGraphicElement();
	if (!obj) return;
	Style neu = getRelatedSceneGraph()->getStyleOf(obj);
	neu.fillPattern(svgPattern->createPattern());
	copyStyleToGraphicElement(neu);
}
//-----------------------------------------------------------------------------
void SvgObject::onStrokeObject(SvgObject::Ptr strokeObj)
{
	SvgPatternBase::Ptr svgPattern = boost::dynamic_pointer_cast<SvgPatternBase>(strokeObj);
	if (!svgPattern)
		return;
	GraphicElement::Ptr obj = getGraphicElement();
	if (!obj) return;
	Style neu = getRelatedSceneGraph()->getStyleOf(obj);
	neu.strokePattern(svgPattern->createPattern());
	copyStyleToGraphicElement(neu);
}
//-----------------------------------------------------------------------------
void SvgObject::set( const SvgObject::Stroke_tag::Type &colorStr,
	const SvgObject::Stroke_tag & )
{
	using sambag::disco::svg::Style;

	// check whether referenced by id
	std::string id = AttributeParser::getUrl(colorStr);
	if (id.length()>0) { // get object by id (via recall)
		SvgRoot::Ptr root = boost::dynamic_pointer_cast<SvgRoot>(this->getRoot());
		SAMBAG_ASSERT(root);
		SvgRoot::ObjectRequestFunction callBk =
			boost::bind(&SvgObject::onStrokeObject, this, _1);
		root->requestForObject(id, callBk);
		return;
	}

	ColorRGBA color;
	AttributeParser::parseColor(colorStr, color);
	if (color==ColorRGBA::NULL_COLOR) return;
	GraphicElement::Ptr obj = getGraphicElement();
	if (!obj) return;
	Style neu = getRelatedSceneGraph()->getStyleOf(obj);
	neu.strokeColor(color);
	copyStyleToGraphicElement(neu);
}
//-----------------------------------------------------------------------------
void SvgObject::set( const SvgObject::Fill_tag::Type &colorStr,
	const SvgObject::Fill_tag & )
{
	// check whether referenced by id
	std::string id = AttributeParser::getUrl(colorStr);
	if (id.length()>0) { // get object by id (via recall)
		SvgRoot::Ptr root = boost::dynamic_pointer_cast<SvgRoot>(this->getRoot());
		SAMBAG_ASSERT(root);
		SvgRoot::ObjectRequestFunction callBk =
			boost::bind(&SvgObject::onFillObject, this, _1);
		root->requestForObject(id, callBk);
		return;
	}

	using sambag::disco::svg::Style;
	ColorRGBA color;
	AttributeParser::parseColor(colorStr, color);
	if (color==ColorRGBA::NULL_COLOR) return;
	GraphicElement::Ptr obj = getGraphicElement();
	if (!obj) return;
	Style neu = getRelatedSceneGraph()->getStyleOf(obj);
	neu.fillColor(color);
	copyStyleToGraphicElement(neu);
}
//-----------------------------------------------------------------------------
void SvgObject::registerAttributes( SvgObject::BuilderType &binder ) {
	binder.registerAttribute
	<StrokeWidth_tag::Type, StrokeWidth_tag, SvgObject> ("stroke-width");
	binder.registerAttribute
	<Stroke_tag::Type, Stroke_tag, SvgObject>("stroke");
	binder.registerAttribute
	<Fill_tag::Type, Fill_tag, SvgObject>("fill");
	binder.registerAttribute
	<Id_tag::Type, Id_tag, SvgObject>("id");
	binder.registerAttribute
	<Class_tag::Type, Class_tag, SvgObject>("class");
	binder.registerAttribute
	<Transform_tag::Type, Transform_tag, SvgObject>("transform");
	binder.registerAttribute
	<OpacityStyle_tag::Type, OpacityStyle_tag, SvgObject>("opacity");
	binder.registerAttribute
	<StrokeOpacityStyle_tag::Type, StrokeOpacityStyle_tag, SvgObject>("stroke-opacity");
	binder.registerAttribute
	<FillOpacityStyle_tag::Type, FillOpacityStyle_tag, SvgObject>("fill-opacity");
	binder.registerAttribute
	<Style_tag::Type, Style_tag, SvgObject>("style");
	binder.registerAttribute
	<DashOffsetStyle_tag::Type, DashOffsetStyle_tag, SvgObject>("stroke-dashoffset");
	binder.registerAttribute
	<DashArrayStyle_tag::Type, DashArrayStyle_tag, SvgObject>("stroke-dasharray");
	binder.registerAttribute
	<FontFamily_tag::Type, FontFamily_tag, SvgObject>("font-family");
	binder.registerAttribute
	<FontSize_tag::Type, FontSize_tag, SvgObject>("font-size");
	binder.registerAttribute
	<FontWeight_tag::Type, FontWeight_tag, SvgObject>("font-weight");
	binder.registerAttribute
	<FontStyle_tag::Type, FontStyle_tag, SvgObject>("font-style");
	binder.registerAttribute
	<FontStyle_tag::Type, FontStyle_tag, SvgObject>("font-style");
	binder.registerAttribute
	<StrokeLineCap_tag::Type, StrokeLineCap_tag, SvgObject>("stroke-linecap");
	binder.registerAttribute
	<StrokeLineJoin_tag::Type, StrokeLineJoin_tag, SvgObject>("stroke-linejoin");
	binder.registerAttribute
	<StrokeMiterLimit::Type, StrokeMiterLimit, SvgObject>("stroke-miterlimit");
	binder.registerAttribute
	<UserData_tag::Type, UserData_tag, SvgObject>("user-data");
}

}}} // namespaces
