/*
 * SvgImage.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef DISCO_SVGIMAGE_HPP_
#define DISCO_SVGIMAGE_HPP_

#include "Svg.hpp"
#include "graphicElements/Image.hpp"
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
	struct X_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Y_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Width_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Height_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct HRef_tag { typedef std::string Type; };
private:
	//-------------------------------------------------------------------------
	std::string svgPath;
protected:
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::Image::Ptr image;
	//-------------------------------------------------------------------------
	SvgImage() {
		image = sambag::disco::svg::graphicElements::Image::create();
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
		if (root) {
			neu->svgPath = root->getSvgPath();		
		}
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgImage() {
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const X_tag::Type &coord, const X_tag& ) {
		image->getOutline().x0().x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y_tag::Type &coord, const Y_tag& ) {
		image->getOutline().x0().y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Width_tag::Type &val, const Width_tag& ) {
		image->getOutline().size().width(val);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Height_tag::Type &val, const Height_tag& ) {
		image->getOutline().size().height(val);
	}
	//-------------------------------------------------------------------------
	virtual void set( const HRef_tag::Type &v, const HRef_tag& ) {
		image->setUri(svgPath+v);
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
