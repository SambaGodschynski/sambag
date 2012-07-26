/*
 * SvgUse.hpp
 *
 *  Created on: 06.10.2011
 *      Author: samba
 */

#ifndef DISCO_SVG_USE_HPP_
#define DISCO_SVG_USE_HPP_

#include "Svg.hpp"
#include "sambag/com/Common.hpp"
#include "graphicElements/RefElement.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
class SvgUse : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgUse> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
	//-------------------------------------------------------------------------
	struct HRef_tag { typedef std::string Type; };
private:
	//-------------------------------------------------------------------------
	std::string href;
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::RefElement::Ptr object;
	//-------------------------------------------------------------------------
	/**
	 * gets the element with the given href id and puts into object.
	 */
	void solveHRef();
	//-------------------------------------------------------------------------
	/**
	 * Rebuilds graph structure starting with the reference target with
	 * reference copies.
	 * @param g
	 */
	void completeReferenceGraph();
protected:
	//-------------------------------------------------------------------------
	SvgUse();
	//-------------------------------------------------------------------------
	virtual void init() {
		solveHRef();
	}
public:
	//-------------------------------------------------------------------------
	virtual ~SvgUse(){}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgUse());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return object;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const HRef_tag::Type &val, HRef_tag ) {
		href=val;
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<HRef_tag::Type, HRef_tag, SvgUse>("xlink:href");
	}
};

}}} // namespaces

#endif /* SVGUSE_HPP_ */
