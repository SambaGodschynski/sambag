/*
 * SvgText.hpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#ifndef SVGTEXT_HPP_
#define SVGTEXT_HPP_
#include "Svg.hpp"
#include "sambag/disco/graphicElements/Text.hpp"


// TODO: tspan


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgText : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgText> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct X_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct Y_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct FontFamily_tag { typedef std::string Type; };
	//-------------------------------------------------------------------------
	struct FontSize_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct FontWeight_tag { typedef Font::Weight Type; };
	//-------------------------------------------------------------------------
	struct FontStyle_tag { typedef Font::Slant Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Text::Ptr text;
	//-------------------------------------------------------------------------
	SvgText();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getDrawingObject() const {
		return text;
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgText());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual void setXmlText( const std::string &str ) {
		if (!text) return;
		text->setText(str);
	}
	//-------------------------------------------------------------------------
	virtual ~SvgText();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		if (!text) return Rectangle();
		return text->getBoundingBox();
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const X_tag::Type &coord, X_tag ) {
		Point2D pos = text->getPos();
		pos.x(coord);
		text->setPos(pos);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y_tag::Type &coord, Y_tag ) {
		Point2D pos = text->getPos();
		pos.y(coord);
		text->setPos(pos);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontFamily_tag::Type &v, FontFamily_tag ) {
		using sambag::disco::graphicElements::Style;
		Style neu = getStyle();
		Font f = neu.font();
		if (f==Style::NO_FONT) f = Font();
		f.fontFace = v;
		neu.font(f);
		setStyle(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontSize_tag::Type &v, FontSize_tag ) {
		using sambag::disco::graphicElements::Style;
		Style neu = getStyle();
		Font f = neu.font();
		if (f==Style::NO_FONT) f = Font();
		f.size = v;
		neu.font(f);
		setStyle(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontWeight_tag::Type &v, FontWeight_tag ) {
		using sambag::disco::graphicElements::Style;
		Style neu = getStyle();
		Font f = neu.font();
		if (f==Style::NO_FONT) f = Font();
		f.weight = v;
		neu.font(f);
		setStyle(neu);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FontStyle_tag::Type &v, FontStyle_tag ) {
		using sambag::disco::graphicElements::Style;
		Style neu = getStyle();
		Font f = neu.font();
		if (f==Style::NO_FONT) f = Font();
		f.slant = v;
		neu.font(f);
		setStyle(neu);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X_tag::Type, X_tag, SvgText>("x");
		binder.registerAttribute<Y_tag::Type, Y_tag, SvgText>("y");
		binder.registerAttribute<FontFamily_tag::Type, FontFamily_tag, SvgText>("font-family");
		binder.registerAttribute<FontSize_tag::Type, FontSize_tag, SvgText>("font-size");
		binder.registerAttribute<FontWeight_tag::Type, FontWeight_tag, SvgText>("font-weight");
		binder.registerAttribute<FontStyle_tag::Type, FontStyle_tag, SvgText>("font-style");
	}
};

}}} // namespaces

#endif /* SVGTEXT_HPP_ */
