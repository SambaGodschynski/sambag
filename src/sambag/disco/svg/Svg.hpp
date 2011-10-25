/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef SVG_GRAPHICELEMENT_HPP_
#define SVG_GRAPHICELEMENT_HPP_


#include "sambag/com/Common.hpp"
#include "sambag/disco/graphicElements/GraphicElement.hpp"
#include "sambag/xml/XML2Obj.hpp"
#include "AttributeParser.hpp"
#include "boost/flyweight.hpp"
#include "sambag/disco/graphicElements/SceneGraph.hpp"
#include "StyleParser.hpp"
#include <list>

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
	//-------------------------------------------------------------------------
	/**
	 * needed for registerAttributes()
	 */
	typedef sambag::xml::XML2Object<SvgObject, SceneGraph> BuilderType;
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
	struct Stroke_tag { typedef ColorRGBA Type; };
	//-------------------------------------------------------------------------
	struct Fill_tag { typedef ColorRGBA Type; };
	//-------------------------------------------------------------------------
	struct Id_tag { typedef IdType Type; };
	//-------------------------------------------------------------------------
	struct Class_tag { typedef ClassType Type; };
	//-------------------------------------------------------------------------
	struct Transform_tag { typedef sambag::com::Matrix Type; };
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
	struct Style_tag { typedef disco::graphicElements::Style Type; };
private:
	//-------------------------------------------------------------------------
	WPtr svgRootObject;
	//-------------------------------------------------------------------------
	std::string tagName;
	//-------------------------------------------------------------------------
	std::string idName;
	//-------------------------------------------------------------------------
	std::string className;
	//-------------------------------------------------------------------------
	graphicElements::SceneGraph::Ptr sceneGraph;
protected:
	//-------------------------------------------------------------------------
	/**
	 * called when finishing @see SvgRoot::subObjectCreated.
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
	copyStyleToGraphicElement(const sambag::disco::graphicElements::Style &b)
	{
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		getRelatedSceneGraph()->setStyleTo(obj, b);
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return __self.lock();
	}
	//-------------------------------------------------------------------------
	void setRelatedSceneGraph(SceneGraph::Ptr ptr) {
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
	void setTransformMatrix( const sambag::com::Matrix &m ) {
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj)
			return;
		getRelatedSceneGraph()->setTransfomationTo(obj, m);
	}
	//-------------------------------------------------------------------------
	virtual void add(Ptr obj);
	//-------------------------------------------------------------------------
	virtual ~SvgObject(){}
	//-------------------------------------------------------------------------
	virtual void setTagName(const std::string & str) { tagName = str; }
	//-------------------------------------------------------------------------
	virtual void setClassName(const std::string & str) { className = str; }
	//-------------------------------------------------------------------------
	virtual void setIdName(const std::string & str) { idName = str; }
	//-------------------------------------------------------------------------
	const std::string & getTagName() const { return tagName; }
	//-------------------------------------------------------------------------
	const std::string & getClassName() const { return className; }
	//-------------------------------------------------------------------------
	const std::string & getIdName() const { return idName; }
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const StrokeWidth_tag::Type &width, const StrokeWidth_tag&)
	{
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.strokeWidth(width);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Stroke_tag::Type &color, const Stroke_tag & ) {
		using sambag::disco::graphicElements::Style;
		if (color==NULL_COLOR) return;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.strokeColor(color);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Fill_tag::Type &color, const Fill_tag & ) {
		using sambag::disco::graphicElements::Style;
		if (color==NULL_COLOR) return;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fillColor(color);
		copyStyleToGraphicElement(neu);
	}
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
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		Number v;
		AttributeParser::parseOpacity(strV, v);
		ColorRGBA fill = neu.fillColor();
		ColorRGBA stroke = neu.strokeColor();
		fill.setA(v); stroke.setA(v);
		neu.fillColor(fill);
		neu.strokeColor(stroke);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Style_tag::Type &style, const Style_tag&)
	{
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		copyStyleToGraphicElement(style);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Font-Style
	//-------------------------------------------------------------------------
	virtual void set( const FontFamily_tag::Type &v, FontFamily_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontFace(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontSize_tag::Type &v, FontSize_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontSize(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontWeight_tag::Type &v, FontWeight_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontWeight(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontStyle_tag::Type &v, FontStyle_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = getRelatedSceneGraph()->getStyleOf(obj);
		neu.fontSlant(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder );
};
}}} // namespace

#endif /* SVGGRAPHICELEMENT_HPP_ */
