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
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Text::Ptr text;
	//-------------------------------------------------------------------------
	SvgText();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
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
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X_tag::Type, X_tag, SvgText>("x");
		binder.registerAttribute<Y_tag::Type, Y_tag, SvgText>("y");
	}
};

}}} // namespaces

#endif /* SVGTEXT_HPP_ */
