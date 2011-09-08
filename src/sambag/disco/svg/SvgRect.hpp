/*
 * SvgRect.hpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#ifndef SVGRECT_HPP_
#define SVGRECT_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Rect.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgRect : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgRect> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct X_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct Y_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct WIDTH_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct HEIGHT_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct RX_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	struct RY_tag { typedef Number Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Rect::Ptr rect;
	//-------------------------------------------------------------------------
	SvgRect() {
		using namespace sambag::disco::graphicElements;
		rect = Rect::create();
	}
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgRect());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return rect;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgRect() {}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const X_tag::Type &coord, X_tag ) {
		Rectangle r = rect->getRectangle();
		Point2D p = r.getX0();
		p.x(coord);
		r.setX0(p);
		rect->setRectangle(r);

	}
	//-------------------------------------------------------------------------
	virtual void set( const Y_tag::Type &coord, Y_tag ) {
		Rectangle r = rect->getRectangle();
		Point2D p = r.getX0();
		p.y(coord);
		r.setX0(p);
		rect->setRectangle(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const WIDTH_tag::Type &w, WIDTH_tag ) {
		Rectangle r = rect->getRectangle();
		r.setWidth(w);
		rect->setRectangle(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const HEIGHT_tag::Type &h, HEIGHT_tag ) {
		Rectangle r = rect->getRectangle();
		r.setHeight(h);
		rect->setRectangle(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const RX_tag::Type &x, RX_tag ) {
		Point2D r = rect->getRadius();
		r.x(x);
		rect->setRadius(r);
	}
	//-------------------------------------------------------------------------
	virtual void set( const RY_tag::Type &y, RY_tag ) {
		Point2D r = rect->getRadius();
		r.y(y);
		rect->setRadius(r);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X_tag::Type, X_tag, SvgRect>("x");
		binder.registerAttribute<Y_tag::Type, Y_tag, SvgRect>("y");
		binder.registerAttribute<WIDTH_tag::Type, WIDTH_tag, SvgRect>("width");
		binder.registerAttribute<HEIGHT_tag::Type, HEIGHT_tag, SvgRect>("height");
		binder.registerAttribute<RX_tag::Type, RX_tag, SvgRect>("rx");
		binder.registerAttribute<RY_tag::Type, RY_tag, SvgRect>("ry");
	}
};

}}} // namespaces

#endif /* SVGRECT_HPP_ */
