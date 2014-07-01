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
void SvgRoot::init() {

}
//-----------------------------------------------------------------------------
SvgRoot::Ptr SvgRoot::create(SvgRoot *root, bool firstElement) {
    Ptr neu(new SvgRoot());
    neu->__setSelf(neu);
    if (!root || root == neu.get())
        return neu;
    neu->createBase(root);
    // set default style if first element
    if (firstElement) {
        graphicElements::Style style = graphicElements::Style::DEFAULT_STYLE;
        style.strokePattern(IPattern::Ptr());
        root->getRelatedSceneGraph()->setStyleTo(neu->getGraphicElement(), style);
    }
    return neu;
}
//-----------------------------------------------------------------------------
SvgRoot::Ptr SvgRoot::create(graphicElements::SceneGraph *g, bool firstElement)
{
    Ptr neu(new SvgRoot());
    neu->__setSelf(neu);
    if (!g)
        return neu;
    neu->setRelatedSceneGraph(g->getPtr());
    // set default style if first element
    if (firstElement) {
        graphicElements::Style style = graphicElements::Style::DEFAULT_STYLE;
        style.strokePattern(IPattern::Ptr());
        g->setStyleTo(neu->getGraphicElement(), style);
    }
    return neu;
}
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
		// call function
		rq.second(it->second.lock());
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
	if (newObject.get() != this) // don't add self, ocurrs memory leak otherwise
		svgs.push_back(newObject);
}

}}}
