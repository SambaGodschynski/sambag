/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef DISCO_SVGLINE_HPP_
#define DISCO_SVGLINE_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Line.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgLine : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgLine> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct X1_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct Y1_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct X2_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct Y2_tag { typedef Coordinate Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Line::Ptr line;
	//-------------------------------------------------------------------------
	SvgLine();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return line;
	}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgLine());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgLine();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const X1_tag::Type &coord, X1_tag ) {
		Point2D p = line->getP0();
		p.x(coord);
		line->setP0(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y1_tag::Type &coord, Y1_tag ) {
		Point2D p = line->getP0();
		p.y(coord);
		line->setP0(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const X2_tag::Type &coord, X2_tag ) {
		Point2D p = line->getP1();
		p.x(coord);
		line->setP1(p);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y2_tag::Type &coord, Y2_tag ) {
		Point2D p = line->getP1();
		p.y(coord);
		line->setP1(p);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X1_tag::Type, X1_tag, SvgLine>("x1");
		binder.registerAttribute<Y1_tag::Type, Y1_tag, SvgLine>("y1");
		binder.registerAttribute<X2_tag::Type, X2_tag, SvgLine>("x2");
		binder.registerAttribute<Y1_tag::Type, Y2_tag, SvgLine>("y2");
	}
};

}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
