/*
 * SvgEllipse.hpp
 *
 *  Created on: 04.10.2011
 *      Author: samba
 */

#ifndef SVGELLIPSE_HPP_
#define SVGELLIPSE_HPP_

#include "Svg.hpp"
#include "graphicElements/Arc.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgEllipse : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgEllipse> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct CX_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct CY_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct RX_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct RY_tag { typedef units::Unit Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::Arc::Ptr ellipse;
	//-------------------------------------------------------------------------
	SvgEllipse();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return ellipse;
	}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgEllipse());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgEllipse();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const CX_tag::Type &coord, CX_tag ) {
		ellipse->getCenter().x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const CY_tag::Type &coord, CY_tag ) {
		ellipse->getCenter().y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const RX_tag::Type &coord, RX_tag ) {
		ellipse->getRadius().x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const RY_tag::Type &coord, RY_tag ) {
		ellipse->getRadius().y(coord);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<CX_tag::Type, CX_tag, SvgEllipse>("cx");
		binder.registerAttribute<CY_tag::Type, CY_tag, SvgEllipse>("cy");
		binder.registerAttribute<RX_tag::Type, RX_tag, SvgEllipse>("rx");
		binder.registerAttribute<RY_tag::Type, RY_tag, SvgEllipse>("ry");
	}
};

}}} // namespace


#endif /* SVGELLIPSE_HPP_ */
