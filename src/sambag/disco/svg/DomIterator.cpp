/*
 * DomIterator.cpp
 *
 *  Created on: 17.10.2011
 *      Author: samba
 */

#include "DomIterator.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class DomIterator
//=============================================================================
//-----------------------------------------------------------------------------
DomIterator::DomIterator(DomObject* obj) : currObj(obj) {
}
//-----------------------------------------------------------------------------
DomIterator::DomIterator(Ptr iterator) : nestedIterator(iterator) {
}
//-----------------------------------------------------------------------------
DomIterator::Ptr DomIterator::allSiblingObjects(DomObject* obj) {
	return Ptr();
}
//-----------------------------------------------------------------------------
DomIterator::Ptr DomIterator::allSiblingObjects(DomIterator::Ptr iterator) {
	return Ptr();
}
}}} // namespace
