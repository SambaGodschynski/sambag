/*
 * DomObject.cpp
 *
 *  Created on: 17.10.2011
 *      Author: samba
 */

#include "DomObject.hpp"
#include "DomIterator.hpp"

namespace sambag { namespace disco { namespace svg {

namespace {
//=============================================================================
class PrivateIterator : public DomIterator {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<PrivateIterator> Ptr;
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	DomObject::DomObjects::iterator it, end;
protected:
	//-------------------------------------------------------------------------
	PrivateIterator(DomObject *obj) {

	}
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	static Ptr allSiblingObjects(DomObject *obj);
	//-------------------------------------------------------------------------
	static Ptr allSiblingObjects(Ptr iterator);
	//-------------------------------------------------------------------------
	virtual DomObject * next() = 0;
	//-------------------------------------------------------------------------
	virtual bool hasNext() const = 0;
};

}


//=============================================================================
// class DomObject
//=============================================================================
//-----------------------------------------------------------------------------
DomIteratorPtr DomObject::allSiblingObjects() {
	return DomIterator::allSiblingObjects(this);
}

}}} // namespaces
