/*
 * SvgLinearGradient.hpp
 *
 *  Created on: Dec 3, 2011
 *      Author: samba
 */

#ifndef SVGLINEARGRADIENT_HPP_
#define SVGLINEARGRADIENT_HPP_

#include "Svg.hpp"
#include "SvgPattern.hpp"
#include "units/Units.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
/**
 * @class SvgLinearGradient
 * has no representation in SceneGraph because its not rendered directly.
 * Provides a method createPattern() which creates a Pattern Object for
 * this gradient.
 */
class SvgLinearGradient : public SvgGradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgLinearGradient> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
private:
	//-------------------------------------------------------------------------
	units::Point x1, x2;
protected:
	//-------------------------------------------------------------------------
	SvgLinearGradient(){
		x1.x().setType(units::Unit::UnitPERCENT);
		x1.y().setType(units::Unit::UnitPERCENT);
		x2.x().setType(units::Unit::UnitPERCENT);
		x2.y().setType(units::Unit::UnitPERCENT);

		x1.x().setValue(0.);
		x1.y().setValue(0.);
		x2.x().setValue(100.);
		x2.y().setValue(0.);
	}
public:
	//-------------------------------------------------------------------------
	struct X1_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Y1_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct X2_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct Y2_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	virtual void add(SvgObject::Ptr obj) {
		SvgColorStop::Ptr stop = boost::dynamic_pointer_cast<SvgColorStop>(obj);
		if (!stop)
			return;
		addColorStop(stop);
	}
	//-------------------------------------------------------------------------
	/**
	 * creates pattern with rect's bounds.
	 * @param rect
	 * @return
	 */
	IPattern::Ptr createPattern() const;
	//-------------------------------------------------------------------------
	virtual ~SvgLinearGradient(){}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgLinearGradient());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return GraphicElement::Ptr(); // return NULL
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const X1_tag::Type &coord, X1_tag ) {
		x1.x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y1_tag::Type &coord, Y1_tag ) {
		x1.y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const X2_tag::Type &coord, X2_tag ) {
		x2.x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const Y2_tag::Type &coord, Y2_tag ) {
		x2.y(coord);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<X1_tag::Type, X1_tag, SvgLinearGradient>("x1");
		binder.registerAttribute<Y1_tag::Type, Y1_tag, SvgLinearGradient>("y1");
		binder.registerAttribute<X2_tag::Type, X2_tag, SvgLinearGradient>("x2");
		binder.registerAttribute<Y1_tag::Type, Y2_tag, SvgLinearGradient>("y2");
	}
};
}}}
#endif /* SVGLINEARGRADIENT_HPP_ */
