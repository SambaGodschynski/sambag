/*
 * SvgCircle.hpp
 *
 *  Created on: 04.10.2011
 *      Author: samba
 */

#ifndef SVGCIRCLE_HPP_
#define SVGCIRCLE_HPP_

#include "Svg.hpp"
#include "graphicElements/Arc.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgCircle : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgCircle> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct CX_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct CY_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct R_tag { typedef units::Unit Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::Arc::Ptr circle;
	//-------------------------------------------------------------------------
	SvgCircle();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return circle;
	}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgCircle());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgCircle();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const CX_tag::Type &coord, CX_tag ) {
		 circle->getCenter().x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const CY_tag::Type &coord, CY_tag ) {
		circle->getCenter().y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const R_tag::Type &coord, R_tag ) {
		circle->getRadius().x(coord);
		circle->getRadius().y().setType(units::Unit::NONE);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<CX_tag::Type, CX_tag, SvgCircle>("cx");
		binder.registerAttribute<CY_tag::Type, CY_tag, SvgCircle>("cy");
		binder.registerAttribute<R_tag::Type, R_tag, SvgCircle>("r");
	}
};

}}} // namespace

#endif /* SVGCIRCLE_HPP_ */
