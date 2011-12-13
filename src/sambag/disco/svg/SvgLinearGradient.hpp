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
protected:
	//-------------------------------------------------------------------------
	SvgLinearGradient(){}
public:
	//-------------------------------------------------------------------------
	virtual void add(SvgObject::Ptr obj) {
		SvgColorStop::Ptr stop = boost::shared_dynamic_cast<SvgColorStop>(obj);
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
	IPattern::Ptr createPattern(const Rectangle &rect) const;
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
	static void registerAttributes( SvgObject::BuilderType &binder ) {

	}
};
}}}
#endif /* SVGLINEARGRADIENT_HPP_ */
