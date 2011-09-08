/*
 * SvgEllipse.hpp
 *
 *  Created on: 04.10.2011
 *      Author: samba
 */

#ifndef SVGELLIPSE_HPP_
#define SVGELLIPSE_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Arc.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgEllipse : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgEllipse> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct CX_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct CY_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct RX_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct RY_tag { typedef Number Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Arc::Ptr ellipse;
	//-------------------------------------------------------------------------
	SvgEllipse();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return ellipse;
	}
	//-------------------------------------------------------------------------
	static Ptr create(graphicElements::SceneGraph *g = NULL) {
		Ptr neu(new SvgEllipse());
		neu->__setSelf(neu);
		if (!g)
			return neu;
		neu->setRelatedSceneGraph(g->getPtr());
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgEllipse();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const CX_tag::Type &coord, CX_tag ) {
		Point2D p = ellipse->getCenter();
		p.x(coord);
		ellipse->setCenter(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const CY_tag::Type &coord, CY_tag ) {
		Point2D p = ellipse->getCenter();
		p.y(coord);
		ellipse->setCenter(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const RX_tag::Type &coord, RX_tag ) {
		Point2D p = ellipse->getRadius();
		p.x(coord);
		ellipse->setRadius(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const RY_tag::Type &coord, RY_tag ) {
		Point2D p = ellipse->getRadius();
		p.y(coord);
		ellipse->setRadius(p);
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
