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
		combo = sambag::disco::graphicElements::Compound::create();
	}
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return combo;
	}
	//-------------------------------------------------------------------------
	size_t getNumChildren() const { return combo->getNumChildren(); }
	//-------------------------------------------------------------------------
	virtual void add(SvgObject::Ptr obj) {
		SvgObject::add(obj);
		using sambag::disco::graphicElements::GraphicElement;
		GraphicElement::Ptr gObj=obj->getGraphicElement();
		if (!gObj)
			return;
		combo->add(gObj);
	}
	//-------------------------------------------------------------------------
	virtual ~SvgCompound(){}
	//-------------------------------------------------------------------------
	static Ptr create(graphicElements::SceneGraph *g = NULL) {
		Ptr neu(new SvgCompound());
		neu->__setSelf(neu);
		if (!g)
			return neu;
		neu->setRelatedSceneGraph(g->getPtr());
		return neu;
	}
};

}}} // namespaces


#endif /* SVG_COMPOUNDELEMENT_HPP_ */
