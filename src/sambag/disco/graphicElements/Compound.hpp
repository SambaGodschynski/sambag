/*
 * CompoundElement.hpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#ifndef COMPOUNDELEMENT_HPP_
#define COMPOUNDELEMENT_HPP_

#include "GraphicElement.hpp"
#include "sambag/com/Common.hpp"
#include <list>

namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
/**
 * Base class for all compounded SVG elements.
 */
class Compound : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Compound> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<GraphicElement::Ptr> GraphicElements;
private:
	//-------------------------------------------------------------------------
	GraphicElements objects;
protected:
	Compound(){}
public:
	//-------------------------------------------------------------------------
	virtual void copyStyleFrom( const Style &b );
	//-------------------------------------------------------------------------
	virtual void setStyle( const Style &_style );
	//-------------------------------------------------------------------------
	size_t getNumChildren() const { return objects.size(); }
	//-------------------------------------------------------------------------
	virtual void add(GraphicElement::Ptr obj) {
		objects.push_back(obj);
	}
	//-------------------------------------------------------------------------
	virtual ~Compound(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const  { return Rectangle(); } // TODO: calc. bounding box
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new Compound());
		neu->__setSelf(neu);
		return neu;
	}
};

}}} // namespaces


#endif /* COMPOUNDELEMENT_HPP_ */
