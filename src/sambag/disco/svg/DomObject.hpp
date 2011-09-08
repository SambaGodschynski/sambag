/*
 * DomObject.hpp
 *
 *  Created on: 17.10.2011
 *      Author: samba
 */

#ifndef DOMOBJECT_HPP_
#define DOMOBJECT_HPP_

#include "sambag/com/Common.hpp"
#include <string>
#include <list>

namespace sambag { namespace xml {
	template <typename T>
	class XML2Object;
}} // namespace

namespace sambag { namespace disco { namespace svg {
//=============================================================================
class SvgObject;
class DomIterator;
typedef boost::shared_ptr<DomIterator> DomIteratorPtr;
//=============================================================================
/**
*  Class DomObject.
*  What it does:
*      - representing the svg structure as object model
*      - accessing to objects via iterators
*  What it not does:
*      - changing objects structure
*      - access to attributes
*/
class DomObject  {
//=============================================================================
friend class sambag::xml::XML2Object<SvgObject>;
friend class PrivateIterator;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<DomObject> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<Ptr> DomObjects;
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	std::string tagName;
	//-------------------------------------------------------------------------
	virtual void setXmlText( const std::string & str) {
		tagName = str;
	}
	//-------------------------------------------------------------------------
	DomObjects children;
protected:
	//-------------------------------------------------------------------------
	void addDomChild(Ptr child) {
		children.push_back(child);
	}
public:
	//-------------------------------------------------------------------------
	DomIteratorPtr allSiblingObjects();

};

}}} // namespace

#endif /* DOMOBJECT_HPP_ */
