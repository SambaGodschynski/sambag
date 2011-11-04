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
#include "sambag/disco/graphicElements/RefElement.hpp"

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
	sambag::disco::graphicElements::RefElement::Ptr object;
	//-------------------------------------------------------------------------
	/**
	 * gets the element with the given href id and puts into object.
	 */
	void solveHRef();
protected:
	//-------------------------------------------------------------------------
	SvgUse();
public:
	//-------------------------------------------------------------------------
	virtual ~SvgUse(){}
	//-------------------------------------------------------------------------
	static Ptr create(graphicElements::SceneGraph *g = NULL) {
		Ptr neu(new SvgUse());
		neu->__setSelf(neu);
		if (!g)
			return neu;
		neu->setRelatedSceneGraph(g->getPtr());
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
		solveHRef();
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<HRef_tag::Type, HRef_tag, SvgUse>("xlink:href");
	}
};

}}} // namespaces

#endif /* SVGUSE_HPP_ */
