/*
 * SvgRoot.hpp.cpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#include "SvgRoot.hpp"
#include "sambag/com/Common.hpp"
#include "boost/any.hpp"
#include <assert.h>
namespace sambag { namespace disco { namespace svg {

//=============================================================================
//class SvgRoot
//=============================================================================
//-----------------------------------------------------------------------------
void SvgRoot::initCreatedObjects() {
	boost_for_each(SvgObject::Ptr o, svgs) {
		o->init();
	}
	creationCompleted = true;
	handleRequests();
}
//-----------------------------------------------------------------------------
void SvgRoot::handleRequests() {
	while (!requests.empty()) {
		ObjectRequest rq = requests.back();
		requests.pop_back();
		IdMap::iterator it = idMap.find(rq.first);
		if (it==idMap.end())
			continue;
		rq.second(it->second);
	}
}
//-----------------------------------------------------------------------------
void SvgRoot::subObjectCreated( SvgObject::Ptr newObject,
	const std::string &tagName )
{
	newObject->setTagName(tagName);
	if (newObject->getIdName().length() > 0) { // append object to idmap
		idMap.insert(std::make_pair(newObject->getIdName(), newObject));
	}
	svgs.push_back(newObject);
}

}}}
