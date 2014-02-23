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
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tag
private:
protected:
	//-------------------------------------------------------------------------
	SvgRadialGradient(){}
public:
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

	}
};
}}}

#endif /* SVGRADIALGRADIENT_H_ */
