/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef DISCO_SVGLINE_HPP_
#define DISCO_SVGLINE_HPP_

#include "Svg.hpp"
#include "graphicElements/Line.hpp"


namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgLine : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgLine> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct X1_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Y1_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct X2_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Y2_tag { typedef units::Unit Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::Line::Ptr line;
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
		line->getP0().x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y1_tag::Type &coord, Y1_tag ) {
		line->getP0().y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const X2_tag::Type &coord, X2_tag ) {
		line->getP1().x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y2_tag::Type &coord, Y2_tag ) {
		line->getP1().y(coord);
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
