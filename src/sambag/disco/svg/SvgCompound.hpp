/*
 * CompoundElement.hpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#ifndef SVG_COMPOUNDELEMENT_HPP_
#define SVG_COMPOUNDELEMENT_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Compound.hpp"
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
	sambag::disco::graphicElements::Compound::Ptr combo;
	//-------------------------------------------------------------------------
	SvgCompound(){
		using sambag::disco::graphicElements::Compound;
		combo = Compound::create();
	}
public:
	//-------------------------------------------------------------------------
	/**
	 * @see Style.copyFrom()
	 * @param _style
	 */
	virtual void copyStyleFrom( const sambag::disco::graphicElements::Style &b ) {
		using namespace sambag::disco::graphicElements;
		GraphicElement::copyStyleFrom(b);
		if (!combo) return;
		combo->copyStyleFrom(b);
	}
	//-------------------------------------------------------------------------
	virtual void setStyle( const sambag::disco::graphicElements::Style &b ) {
		using namespace sambag::disco::graphicElements;
		GraphicElement::setStyle(b);
		if (!combo) return;
		//only copyStyleFrom for sub objects !!
		combo->copyStyleFrom(b);
	}
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getDrawingObject() const {
		return combo;
	}
	//-------------------------------------------------------------------------
	size_t getNumChildren() const { return combo->getNumChildren(); }
	//-------------------------------------------------------------------------
	virtual void add(SvgObject::Ptr obj) {
		combo->add(obj);
	}
	//-------------------------------------------------------------------------
	virtual ~SvgCompound(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const  { return combo->getBoundingBox(); }
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgCompound());
		neu->__setSelf(neu);
		return neu;
	}
};

}}} // namespaces


#endif /* SVG_COMPOUNDELEMENT_HPP_ */
