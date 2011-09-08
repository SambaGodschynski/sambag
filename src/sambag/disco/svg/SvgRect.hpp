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
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Rect::Ptr rect;
	//-------------------------------------------------------------------------
	SvgRect();
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
	virtual ~SvgRect();
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
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X_tag::Type, X_tag, SvgRect>("x");
		binder.registerAttribute<Y_tag::Type, Y_tag, SvgRect>("y");
		binder.registerAttribute<WIDTH_tag::Type, WIDTH_tag, SvgRect>("width");
		binder.registerAttribute<HEIGHT_tag::Type, HEIGHT_tag, SvgRect>("height");
	}
};

}}} // namespaces

#endif /* SVGRECT_HPP_ */
