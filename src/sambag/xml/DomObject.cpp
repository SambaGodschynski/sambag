/*
 * DomObject.cpp
 *
 *  Created on: 17.10.2011
 *      Author: samba
 */

#include "DomObject.hpp"
#include "DomIterator.hpp"

namespace sambag { namespace xml {
//=============================================================================
// class DomObject
//=============================================================================
//-----------------------------------------------------------------------------
void DomObject::find(const std::string &expr, DomObjects &outList) {
	DomIterator it(children.begin(), children.end());
}

}} // namespaces
