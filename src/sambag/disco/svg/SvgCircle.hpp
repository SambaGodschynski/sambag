/*
 * SvgCircle.hpp
 *
 *  Created on: 04.10.2011
 *      Author: samba
 */

#ifndef SVGCIRCLE_HPP_
#define SVGCIRCLE_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Arc.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgCircle : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgCircle> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct CX_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct CY_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct R_tag { typedef Coordinate Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Arc::Ptr circle;
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
		Point2D p = circle->getCenter();
		p.x(coord);
		circle->setCenter(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const CY_tag::Type &coord, CY_tag ) {
		Point2D p = circle->getCenter();
		p.y(coord);
		circle->setCenter(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const R_tag::Type &coord, R_tag ) {
		Point2D p = circle->getRadius();
		p.x(coord);
		p.y(coord);
		circle->setRadius(p);
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
