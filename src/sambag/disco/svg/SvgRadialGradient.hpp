/*
 * SvgRadialGradient.h
 *
 *  Created on: Dec 6, 2011
 *      Author: samba
 */

#ifndef SVGRADIALGRADIENT_H_
#define SVGRADIALGRADIENT_H_

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
class SvgRadialGradient : public SvgGradient {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgRadialGradient> Ptr;
	//-------------------------------------------------------------------------
	struct CX_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct CY_tag { typedef units::Unit Type; };
	//------------------------------------------------------------------------
	struct R_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct FX_tag { typedef units::Unit Type; };
	//-------------------------------------------------------------------------
	struct FY_tag { typedef units::Unit Type; };
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
private:
	//-------------------------------------------------------------------------
	units::Point c, f;
	units::Unit r;
protected:
	//-------------------------------------------------------------------------
	SvgRadialGradient(){
		c.x().setType(units::Unit::PERCENT);
		c.y().setType(units::Unit::PERCENT);
		f.x().setType(units::Unit::PERCENT);
		f.y().setType(units::Unit::PERCENT);
		r.setType(units::Unit::PERCENT);
		c.x().setValue(50.);
		c.y().setValue(50.);
		f.x().setValue(50.);
		f.y().setValue(50.);
		r.setValue(50.);
	}
public:
	//-------------------------------------------------------------------------
	virtual void set( const CX_tag::Type &coord, CX_tag ) {
		c.x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const CY_tag::Type &coord, CY_tag ) {
		c.y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FX_tag::Type &coord, FX_tag ) {
		f.x(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const FY_tag::Type &coord, FY_tag ) {
		f.y(coord);
	}
	//-------------------------------------------------------------------------
	virtual void set( const R_tag::Type &coord, R_tag ) {
		r = coord;
	}
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
	virtual ~SvgRadialGradient(){}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgRadialGradient());
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
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<CX_tag::Type, CX_tag, SvgRadialGradient>("cx");
		binder.registerAttribute<CY_tag::Type, CY_tag, SvgRadialGradient>("cy");
		binder.registerAttribute<FX_tag::Type, FX_tag, SvgRadialGradient>("fx");
		binder.registerAttribute<FY_tag::Type, FY_tag, SvgRadialGradient>("fy");
		binder.registerAttribute<R_tag::Type, R_tag, SvgRadialGradient>("r");
	}
};
}}}

#endif /* SVGRADIALGRADIENT_H_ */
