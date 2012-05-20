/*
 * SvgImage.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef DISCO_SVGIMAGE_HPP_
#define DISCO_SVGIMAGE_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Image.hpp"
#include "AttributeParser.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgImage : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgImage> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct X_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct Y_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct Width_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct Height_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct HRef_tag { typedef std::string Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Image::Ptr image;
	//-------------------------------------------------------------------------
	SvgImage() {
		image = sambag::disco::graphicElements::Image::create();
	}
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return image;
	}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgImage());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgImage() {
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const X_tag::Type &coord, const X_tag& ) {
		Rectangle r = image->getOutline();
		Point2D p = r.x0();
		p.x(coord);
		r.translate(p);
		image->setOutline(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y_tag::Type &coord, const Y_tag& ) {
		Rectangle r = image->getOutline();
		Point2D p = r.x0();
		p.y(coord);
		r.translate(p);
		image->setOutline(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Width_tag::Type &val, const Width_tag& ) {
		Rectangle r = image->getOutline();
		r.setWidth(val);
		image->setOutline(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Height_tag::Type &val, const Height_tag& ) {
		Rectangle r = image->getOutline();
		r.setHeight(val);
		image->setOutline(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const HRef_tag::Type &v, const HRef_tag& ) {
		image->setUri(v);
	}
	//-------------------------------------------------------------------------
	/**
	 * @override
	 * @param strV
	 * @param
	 */
	virtual void set( const OpacityStyle_tag::Type &strV, const OpacityStyle_tag&)
	{
		using namespace sambag::disco;
		Number v;
		AttributeParser::parseOpacity(strV, v);
		image->setOpacity(v);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X_tag::Type, X_tag, SvgImage>("x");
		binder.registerAttribute<Y_tag::Type, Y_tag, SvgImage>("y");
		binder.registerAttribute<Width_tag::Type, Width_tag, SvgImage>("width");
		binder.registerAttribute<Height_tag::Type, Height_tag, SvgImage>("height");
		binder.registerAttribute<HRef_tag::Type, HRef_tag, SvgImage>("xlink:href");
	}
};

}}} // namespace

#endif /* SVGIMAGE_HPP_ */
