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

namespace sambag { namespace disco { namespace svg { namespace graphicElements {

//=============================================================================
/**
 * Dummyclass for compound elements in SceneGraph.
 */
class Compound : public GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Compound> Ptr;
private:
protected:
	Compound(){}
public:
	//-------------------------------------------------------------------------
	std::string toString() const {
			return "Compound";
	}
	//-------------------------------------------------------------------------
	virtual GraphicElement::Ptr clone() const;
	//-------------------------------------------------------------------------
	virtual ~Compound(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new Compound());
		neu->__setSelf(neu);
		return neu;
	}
};

}}}} // namespaces


#endif /* COMPOUNDELEMENT_HPP_ */
