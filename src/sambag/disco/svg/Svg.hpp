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


// TODO: more elements

namespace sambag { namespace disco { namespace svg {
class SvgRoot;
//=============================================================================
/** 
*  SVG version of GraphicElement. SvgObjects doesn't draw anything.
*  But they mostly contains one GraphicElement object that do it.
*/
class SvgObject : public sambag::disco::graphicElements::GraphicElement {
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
	//-------------------------------------------------------------------------
	/**
	 * the object transformations matrix
	 */
	sambag::com::Matrix tMatrix; // TODO: make flyweight
protected:
	//-------------------------------------------------------------------------
	SvgObject() : _id(NULL_ID), _class(NULL_CLASS) {
		using namespace boost::numeric::ublas;
		tMatrix = IDENTITY_MATRIX;
	}
public:
	//-------------------------------------------------------------------------
	Ptr getRoot() const {
		return svgRootObject.lock();
	}
	//-------------------------------------------------------------------------
	/**
	 * SvgObjects doesn't draw anything.
	 * But they mostly contains one GraphicElement object that do it.
	 * @return that GraphicElement that draw the SVG object.
	 */
	virtual GraphicElement::Ptr getDrawingObject() const = 0;
	//-------------------------------------------------------------------------
	void setTransformMatrix( const sambag::com::Matrix &m ) {
		tMatrix = m;
	}
	//-------------------------------------------------------------------------
	const sambag::com::Matrix & getTransformMatrix() const {
		return tMatrix;
	}
	//-------------------------------------------------------------------------
	virtual void add(Ptr obj) {}
	//-------------------------------------------------------------------------
	virtual ~SvgObject(){}
	//-------------------------------------------------------------------------
	virtual void setXmlText( const std::string & str) {}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const  = 0;
	//-------------------------------------------------------------------------
	const IdType & getId() { return _id; }
	//-------------------------------------------------------------------------
	const ClassType & getClass() { return _class; }
	//-------------------------------------------------------------------------
	/**
	 * @see Style.copyFrom()
	 * @param _style
	 */
	virtual void copyStyleFrom( const sambag::disco::graphicElements::Style &b ) {
		using namespace sambag::disco::graphicElements;
		GraphicElement::copyStyleFrom(b);
		GraphicElement::Ptr obj = getDrawingObject();
		if (!obj) return;
		obj->copyStyleFrom(b);
	}
	//-------------------------------------------------------------------------
	virtual void setStyle( const sambag::disco::graphicElements::Style &b ) {
		using namespace sambag::disco::graphicElements;
		GraphicElement::setStyle(b);
		GraphicElement::Ptr obj = getDrawingObject();
		if (!obj) return;
		obj->setStyle(b);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const StrokeWidth_tag::Type &width, const StrokeWidth_tag&)
	{
		using sambag::disco::graphicElements::Style;
		Style neu = getStyle();
		neu.strokeWidth(width);
		setStyle(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Stroke_tag::Type &color, const Stroke_tag & ) {
		using sambag::disco::graphicElements::Style;
		if (color==NULL_COLOR) return;
		Style neu = getStyle();
		neu.strokeColor(color);
		setStyle(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Fill_tag::Type &color, const Fill_tag & ) {
		using sambag::disco::graphicElements::Style;
		if (color==NULL_COLOR) return;
		Style neu = getStyle();
		neu.fillColor(color);
		setStyle(neu);
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
	static void registerAttributes( SvgObject::BuilderType &binder );
};
//=============================================================================
/**
*  Helper class which sets transformation of given SvgObject and
*  automatically resets when object is destroying.
*  Usage:
*
*    draw(IContext::Ptr cn) {
*      AutoTransform at(object, cn) // set transformation of object on cn
*      // ... do your drawing stuff
*      // ...
*      // transformation will be automatically reseted on the end of scope
*    }
*/
class AutoTransform {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	const sambag::com::Matrix &matrix;
	//-------------------------------------------------------------------------
	IDrawContext::Ptr cn;
	//-------------------------------------------------------------------------
	sambag::com::Matrix tmp;
public:
	//-------------------------------------------------------------------------
	AutoTransform( const sambag::com::Matrix &matrix, IDrawContext::Ptr cn ) :
		matrix(matrix), cn(cn), tmp( sambag::com::Matrix(3,3) )
	{
		cn->getMatrix(tmp);
		cn->transform(matrix);
	}
	//-------------------------------------------------------------------------
	~AutoTransform() {
		cn->identityMatrix();
		cn->transform(tmp);
	}
};

}}} // namespace

#endif /* SVGGRAPHICELEMENT_HPP_ */
