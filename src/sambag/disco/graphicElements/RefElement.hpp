/*
 * RefElement.hpp
 *
 *  Created on: 07.10.2011
 *      Author: samba
 */

#ifndef REFELEMENT_HPP_
#define REFELEMENT_HPP_

#include "sambag/disco/graphicElements/GraphicElement.hpp"
#include "SceneGraph.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
/**
 * Class RefElement.
 * Graphic element which contains another GraphicElement to draw.
 */
class RefElement : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RefElement> Ptr;
private:
	//-------------------------------------------------------------------------
	IDrawable::Ptr ref;
protected:
	//-------------------------------------------------------------------------
	RefElement(){}
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
		return "RefElement["+ (!ref ? "NULL" : "&" + ref->toString()) +"]";
	}
	//-------------------------------------------------------------------------
	virtual GraphicElement::Ptr clone() const {
		Ptr neu = create();
		*neu = *this;
		return neu;
	}
	//-------------------------------------------------------------------------
	static Ptr create()
	{
		Ptr neu(new RefElement());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	void setReference(IDrawable::Ptr r) {
		ref = r;
	}
	//-------------------------------------------------------------------------
	IDrawable::Ptr getReference() const  { return ref; }
	//-------------------------------------------------------------------------
	virtual ~RefElement(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) {
		if (ref)
			ref->draw(context);
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		if (!ref) return NULL_RECTANGLE;
		return ref->getBoundingBox();
	}
};

}}} // namespace


#endif /* REFELEMENT_HPP_ */
