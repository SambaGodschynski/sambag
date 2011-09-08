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
#include "sambag/disco/svg/StyleParser.hpp"
#include <list>


// TODO: transformation
// TODO: more elements


namespace sambag { namespace disco { namespace svg {

//=============================================================================
/** 
*  SVG version of GraphicElement. SvgObjects doesn't draw anything.
*  But they mostly contains one GraphicElement object that do it.
*/
class SvgObject : public sambag::disco::graphicElements::GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef sambag::xml::XML2Object<SvgObject> BuilderType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgObject> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<SvgObject::Ptr> SvgObjects;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct StrokeWidth_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct Stroke_tag { typedef ColorRGBA Type; };
	//-------------------------------------------------------------------------
	struct Fill_tag { typedef ColorRGBA Type; };
	// TODO: more style tags

private:
protected:
	//-------------------------------------------------------------------------
	SvgObject(){}
public:
	//-------------------------------------------------------------------------
	/**
	 * SvgObjects doesn't draw anything.
	 * But they mostly contains one GraphicElement object that do it.
	 * @return that GraphicElement that draw the SVG object.
	 */
	virtual GraphicElement::Ptr getDrawingObject() const = 0;
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
	static void registerAttributes( SvgObject::BuilderType &binder );
};

}}} // namespace

#endif /* SVGGRAPHICELEMENT_HPP_ */
