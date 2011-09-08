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
#include <list>


// TODO: viewbox
// TODO: fillpatterns
// TODO: styletag
// TODO: css

namespace sambag { namespace disco { namespace svg {
class SvgRoot;
//=============================================================================
/** 
*  Class SVG.
*  SvgObjects are for creating Graphic Elements
*  via XML2Object.
*/
class SvgObject  {
//=============================================================================
friend class SvgRoot;
public:
	//-------------------------------------------------------------------------
	/**
	 * needed for registerAttributes()
	 */
	typedef sambag::xml::XML2Object<SvgObject> BuilderType;
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
	// TODO: more style tags
private:
	//-------------------------------------------------------------------------
	WPtr svgRootObject;
	//-------------------------------------------------------------------------
	IdType _id;
	//-------------------------------------------------------------------------
	ClassType _class;
	//-------------------------------------------------------------------------
	static IdType NULL_ID;
	//-------------------------------------------------------------------------
	static IdType NULL_CLASS;
protected:
	//-------------------------------------------------------------------------
	/**
	 * called when finishing @see SvgRoot::subObjectCreated.
	 */
	virtual void init(){}
	//-------------------------------------------------------------------------
	SvgObject() : _id(NULL_ID), _class(NULL_CLASS) {}
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
		obj->copyStyleFrom(b);
	}
	//-------------------------------------------------------------------------
	Ptr getRoot() const {
		return svgRootObject.lock();
	}
	//-------------------------------------------------------------------------
	/**
	* @return the GraphicElement object which is contained by the SVG object
	*/
	virtual GraphicElement::Ptr getGraphicElement() const = 0;
	//-------------------------------------------------------------------------
	void setTransformMatrix( const sambag::com::Matrix &m ) {
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj)
			return;
		return obj->setTransformMatrix(m);
	}
	//-------------------------------------------------------------------------
	const sambag::com::Matrix & getTransformMatrix() const {
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj)
			return NULL_MATRIX;
		return obj->getTransformMatrix();
	}
	//-------------------------------------------------------------------------
	virtual void add(Ptr obj) {}
	//-------------------------------------------------------------------------
	virtual ~SvgObject(){}
	//-------------------------------------------------------------------------
	virtual void setXmlText( const std::string & str) {}
	//-------------------------------------------------------------------------
	const IdType & getId() { return _id; }
	//-------------------------------------------------------------------------
	const ClassType & getClass() { return _class; }
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const StrokeWidth_tag::Type &width, const StrokeWidth_tag&)
	{
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.strokeWidth(width);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Stroke_tag::Type &color, const Stroke_tag & ) {
		using sambag::disco::graphicElements::Style;
		if (color==NULL_COLOR) return;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.strokeColor(color);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Fill_tag::Type &color, const Fill_tag & ) {
		using sambag::disco::graphicElements::Style;
		if (color==NULL_COLOR) return;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.fillColor(color);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Id_tag::Type &v, const Id_tag&)
	{
		_id = v;
	}
	//-------------------------------------------------------------------------
	virtual void set( const Class_tag::Type &v, const Class_tag&)
	{
		_class = v;
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
		Style neu = obj->getStyle();
		Number v;
		AttributeParser::parseOpacity(strV, v);
		neu.opacity(v);
		copyStyleToGraphicElement(neu);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Font-Style
	//-------------------------------------------------------------------------
	virtual void set( const FontFamily_tag::Type &v, FontFamily_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.fontFace(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontSize_tag::Type &v, FontSize_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.fontSize(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontWeight_tag::Type &v, FontWeight_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.fontWeight(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontStyle_tag::Type &v, FontStyle_tag ) {
		using sambag::disco::graphicElements::Style;
		GraphicElement::Ptr obj = getGraphicElement();
		if (!obj) return;
		Style neu = obj->getStyle();
		neu.fontSlant(v);
		copyStyleToGraphicElement(neu);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder );
};
}}} // namespace

#endif /* SVGGRAPHICELEMENT_HPP_ */
