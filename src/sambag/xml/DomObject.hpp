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
//=============================================================================
class DomIterator;
//=============================================================================
/**
*  @class DomObject.
*  What it does:
*      - representing the svg structure as object model
*      - accessing to objects via css style expressions such as:".classA object"
*  What it not does:
*      - changing objects structure
*      - access to attributes
*/
class DomObject  {
//=============================================================================
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
	std::string idName;
	//-------------------------------------------------------------------------
	std::string className;
	//-------------------------------------------------------------------------
	DomObjects children;
protected:
	//-------------------------------------------------------------------------
	void addDomChild(Ptr child) {
		children.push_back(child);
	}
public:
	//-------------------------------------------------------------------------
	void find(const std::string &expr, DomObjects &outList);
	//-------------------------------------------------------------------------
	virtual void setTagName(const std::string & str) {
		tagName = str;
	}
	//-------------------------------------------------------------------------
	virtual void setClassName(const std::string & str) {
		idName = str;
	}
	//-------------------------------------------------------------------------
	virtual void setIdName(const std::string & str) {
		className = str;
	}
	//-------------------------------------------------------------------------
	const std::string & getTagName() const {
		return tagName;
	}
	//-------------------------------------------------------------------------
	const std::string & getClassName() const {
		return className;
	}
	//-------------------------------------------------------------------------
	const std::string & getIdName() const {
		return idName;
	}
};

}} // namespace

#endif /* DOMOBJECT_HPP_ */
