/*
 * SvgColorStop.hpp
 *
 *  Created on: Dec 5, 2011
 *      Author: samba
 */

#ifndef SVGCOLORSTOP_HPP_
#define SVGCOLORSTOP_HPP_

#include "Svg.hpp"
#include "sambag/disco/IPattern.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgColorStop : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgColorStop> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct Offset_tag { typedef Coordinate Type; };
	//-------------------------------------------------------------------------
	struct Color_tag { typedef ColorRGBA Type; };
	//-------------------------------------------------------------------------
	struct Opacity_tag { typedef Number Type; };
private:
	//-------------------------------------------------------------------------
	IGradient::ColorStop stop;
protected:
	//-------------------------------------------------------------------------
	SvgColorStop(){
		ColorRGBA &col = boost::get<IGradient::COLOR>(stop);
		col.setA(1.0);
	}
public:
	//-------------------------------------------------------------------------
	const IGradient::ColorStop & getStop() const {
		return stop;
	}
	//-------------------------------------------------------------------------
	svg::GraphicElement::Ptr getGraphicElement() const {
		return GraphicElement::Ptr();
	}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgColorStop());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgColorStop() {}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const Offset_tag::Type &val, Offset_tag ) {
		boost::get<IGradient::OFFSET>(stop) = val/100.0;
	}
	//-------------------------------------------------------------------------
	virtual void set( const Color_tag::Type &val, Color_tag ) {
		ColorRGBA &col = boost::get<IGradient::COLOR>(stop);
		col.setR(val.getR());
		col.setG(val.getG());
		col.setB(val.getB());
	}
	//-------------------------------------------------------------------------
	virtual void set( const Opacity_tag::Type &val, Opacity_tag ) {
		ColorRGBA &col = boost::get<IGradient::COLOR>(stop);
		col.setA(val);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<Offset_tag::Type, Offset_tag, SvgColorStop>("offset");
		binder.registerAttribute<Color_tag::Type, Color_tag, SvgColorStop>("stop-color");
		binder.registerAttribute<Opacity_tag::Type, Opacity_tag, SvgColorStop>("stop-opacity");
	}
};

}}} // namespace

#endif /* SVGCOLORSTOP_HPP_ */
