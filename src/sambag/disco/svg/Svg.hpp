/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef SVG_GRAPHICELEMENT_HPP_
#define SVG_GRAPHICELEMENT_HPP_


#include "sambag/com/Common.hpp"
#include "graphicElements/GraphicElement.hpp"
#include "AttributeParser.hpp"
#include "boost/flyweight.hpp"
#include "graphicElements/SceneGraph.hpp"
#include "StyleParser.hpp"
#include <list>
#include "units/Units.hpp"

#include "sambag/xml/XML2Obj.hpp"

namespace sambag { namespace disco { namespace svg {
class SvgRoot;
//=============================================================================
/** 
*  @class SVG.
*  SvgObjects are for creating Graphic Elements
*  via XML2Object.
*/
class SvgObject {
//=============================================================================
friend class SvgRoot;
public:
	/**
	 * needed for registerAttributes()
	 */
	typedef sambag::xml::XML2Object<
		SvgObject, 
		sambag::xml::SharedWithClosure<SvgRoot*>::Creator
	> BuilderType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgObject> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<SvgObject> WPtr;
	//-------------------------------------------------------------------------
	typedef std::list<SvgObject::Ptr> SvgObjects;
	//-------------------------------------------------------------------------
	typedef std::string IdType;
	//-------------------------------------------------------------------------
	typedef std::string ClassType;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct StrokeWidth_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct Stroke_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct Fill_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct Id_tag { typedef IdType Type; };
	//-------------------------------------------------------------------------
	struct Class_tag { typedef ClassType Type; };
	//-------------------------------------------------------------------------
	struct Transform_tag { typedef sambag::math::Matrix Type; };
	//-------------------------------------------------------------------------
	struct FontFamily_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct FontSize_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct FontWeight_tag { typedef Font::Weight Type; };
	//-------------------------------------------------------------------------
	struct FontStyle_tag { typedef Font::Slant Type; };
	//-------------------------------------------------------------------------
	struct OpacityStyle_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct StrokeOpacityStyle_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct FillOpacityStyle_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct DashArrayStyle_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct DashOffsetStyle_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct Style_tag { typedef disco::svg::Style Type; };
	//-------------------------------------------------------------------------
	struct StrokeLineCap_tag { typedef IDrawContext::LineCapStyle Type; };
	//-------------------------------------------------------------------------
	struct StrokeLineJoin_tag { typedef IDrawContext::LineJoin Type; };
	//-------------------------------------------------------------------------
	struct StrokeMiterLimit { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct UserData_tag { typedef std::string Type; };
private:
	//-------------------------------------------------------------------------
	/**
	 * will be called on fill object request (by url).
	 * @param obj
	 */
	void onFillObject(SvgObject::Ptr obj);
	//-------------------------------------------------------------------------
	/**
	* will be called on stroke object request (by url).
	* @param obj
	*/
	void onStrokeObject(SvgObject::Ptr obj);
	//-------------------------------------------------------------------------
	WPtr svgRootObject;
protected:
	//-------------------------------------------------------------------------
	void createBase(SvgRoot *root = NULL);
	//-------------------------------------------------------------------------
	graphicElements::SceneGraph::Ptr sceneGraph;
	//-------------------------------------------------------------------------
	/**
	 * called when SceneGraph building is completed.
	 */
	virtual void init(){}
	//-------------------------------------------------------------------------
	SvgObject() {}
	//-------------------------------------------------------------------------
	boost::weak_ptr<SvgObject> __self;
	//-------------------------------------------------------------------------
	void __setSelf( Ptr self ) { __self = self; }
public:
	//-------------------------------------------------------------------------
	virtual void
	copyStyleToGraphicElement(const sambag::disco::svg::Style &b)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		getRelatedSceneGraph()->setStyleTo(obj, b);
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return __self.lock();
	}
	//-------------------------------------------------------------------------
	/**
	 * set related Scenegraph to element and add it to graph.
	 * @param ptr
	 */
	virtual void setRelatedSceneGraph(SceneGraph::Ptr ptr) {
		sceneGraph = ptr;
		if (sceneGraph)
			sceneGraph->addElement(getGraphicElement());
	}
	//-------------------------------------------------------------------------
	SceneGraph::Ptr getRelatedSceneGraph() const {
		return sceneGraph;
	}
	//-------------------------------------------------------------------------
	Ptr getRoot() const {
		return svgRootObject.lock();
	}
	//-------------------------------------------------------------------------
	virtual void setXmlText(const std::string &str) {}
	//-------------------------------------------------------------------------
	/**
	* @return the GraphicElement object which is contained by the SVG object
	*/
	virtual GraphicElement::Ptr getGraphicElement() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * @param m
	 */
	void setTransformMatrix( const sambag::math::Matrix &m ) {
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj)
			return;
		getRelatedSceneGraph()->setTransfomationTo(obj, m);
	}
	//-------------------------------------------------------------------------
	/**
	 * called by @see sambag::xml::XML2Object
	 * @param obj
	 */
	virtual void add(Ptr obj);
	//-------------------------------------------------------------------------
	virtual ~SvgObject(){}
	//-------------------------------------------------------------------------
	virtual void setTagName(const std::string & str) {
		sceneGraph->setTagName(getGraphicElement(), str);
	}
	//-------------------------------------------------------------------------
	virtual void setClassName(const std::string & str);
	//-------------------------------------------------------------------------
	virtual void setIdName(const std::string & str) {
		sceneGraph->registerElementId(getGraphicElement(), "#" + str);
	}
	//-------------------------------------------------------------------------
	std::string getTagName() const {
		return sceneGraph->getTagName(getGraphicElement());
	}
	//-------------------------------------------------------------------------
	std::string getClassName() const {
		std::list<std::string> l;
		sceneGraph->getClassNames(getGraphicElement(), l);
		return l.back();
	}
	//-------------------------------------------------------------------------
	std::string getIdName() const {
		return sceneGraph->getIdName(getGraphicElement());
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const StrokeWidth_tag::Type &width, const StrokeWidth_tag&)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.strokeWidth(width);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Stroke_tag::Type &color, const Stroke_tag & );
	//-------------------------------------------------------------------------
	virtual void set( const Fill_tag::Type &color, const Fill_tag & );
	//-------------------------------------------------------------------------
	virtual void set( const Id_tag::Type &v, const Id_tag&)
	{
		setIdName(v);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Class_tag::Type &v, const Class_tag&)
	{
		setClassName(v);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Transform_tag::Type &v, const Transform_tag&)
	{
		setTransformMatrix(v);
	}
	//-------------------------------------------------------------------------
	virtual void set( const OpacityStyle_tag::Type &strV, const OpacityStyle_tag&)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		Number v;
		AttributeParser::parseOpacity(strV, v);
		neu.strokeOpacity(v);
		neu.fillOpacity(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const StrokeOpacityStyle_tag::Type &strV,
			const StrokeOpacityStyle_tag&)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		Number v;
		AttributeParser::parseOpacity(strV, v);
		neu.strokeOpacity(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FillOpacityStyle_tag::Type &strV,
			const FillOpacityStyle_tag&)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		Number v;
		AttributeParser::parseOpacity(strV, v);
		neu.fillOpacity(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Style_tag::Type &style, const Style_tag&)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		copyStyleToGraphicElement(style);
	}
	//-------------------------------------------------------------------------
	virtual void set( const UserData_tag::Type &data, const UserData_tag&)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		obj->setUserData(data);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Font-Style
	//-------------------------------------------------------------------------
	virtual void set( const FontFamily_tag::Type &v, const FontFamily_tag & ) {
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontFace(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontSize_tag::Type &v, const FontSize_tag & ) {
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontSize(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontWeight_tag::Type &v, const FontWeight_tag & ) {
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontWeight(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontStyle_tag::Type &v, const FontStyle_tag & ) {
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontSlant(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const DashArrayStyle_tag::Type &v, const DashArrayStyle_tag &) {
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		Dash::Ptr dash = AttributeParser::parseDashArray(v);
		if (!dash)
			return;
		if (neu.dash()) { // copy offset
			dash->offset(neu.dash()->offset());
		}
		neu.dash(dash);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const DashOffsetStyle_tag::Type &v, const DashOffsetStyle_tag &) {
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		Dash::Ptr dash = neu.dash();
		if (!dash)
			dash = Dash::create();
		dash->offset(v);
	}
	//-------------------------------------------------------------------------
	virtual void set( const StrokeLineCap_tag::Type &v,
			const StrokeLineCap_tag &)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.lineCapStyle(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const StrokeLineJoin_tag::Type &v,
			const StrokeLineJoin_tag &)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.lineJoin(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const StrokeMiterLimit::Type &v,
			const StrokeMiterLimit &)
	{
		using sambag::disco::svg::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.miterLimit(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder );
};
//=============================================================================
}}} // namespace

#endif /* SVGGRAPHICELEMENT_HPP_ */
