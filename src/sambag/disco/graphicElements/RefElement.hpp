/*
 * RefElement.hpp
 *
 *  Created on: 07.10.2011
 *      Author: samba
 */

#ifndef REFELEMENT_HPP_
#define REFELEMENT_HPP_

#include "sambag/disco/graphicElements/GraphicElement.hpp"

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
	GraphicElement::Ptr ref;
protected:
	//-------------------------------------------------------------------------
	RefElement(){}
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
		return "RefElement["+ !ref ? "NULL" : ref->toString() +"]";
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
	void setReference(GraphicElement::Ptr r)  { ref = r; }
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getReference() const  { return ref; }
	//-------------------------------------------------------------------------
	virtual ~RefElement(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) {
		ref->draw(context);
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		if (!ref) return Rectangle();
		return ref->getBoundingBox();;
	}
};

}}} // namespace


#endif /* REFELEMENT_HPP_ */
