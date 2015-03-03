/*
 * CompoundElement.hpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#ifndef SVG_COMPOUNDELEMENT_HPP_
#define SVG_COMPOUNDELEMENT_HPP_

#include "Svg.hpp"
#include "graphicElements/Compound.hpp"
#include "sambag/com/Common.hpp"
#include <list>

namespace sambag { namespace disco { namespace svg {

//=============================================================================
/**
 * Base class for all compounded SVG elements.
 */
class SvgCompound : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgCompound> Ptr;
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::Compound::Ptr combo;
	//-------------------------------------------------------------------------
	SvgCompound(){
		combo = sambag::disco::svg::graphicElements::Compound::create();
	}
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return combo;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgCompound(){}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgCompound());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
};

}}} // namespaces


#endif /* SVG_COMPOUNDELEMENT_HPP_ */
