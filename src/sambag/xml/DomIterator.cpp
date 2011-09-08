/*
 * DomIterator.cpp
 *
 *  Created on: 17.10.2011
 *      Author: samba
 */

#include "DomIterator.hpp"

namespace sambag { namespace xml {
//=============================================================================
// class DomIterator
//=============================================================================
//-----------------------------------------------------------------------------
void DomIterator::next() {
	while (*this!=end) { // iterate until found or end
		if (searchFor==NOTHING)
			return;
		std::string str;
		switch (searchFor) {
			case CLASSES: str=(**this)->getClassName(); break;
			case TAGS:    str=(**this)->getTagName(); break;
			default: return;
		}
		if ( boost::regex_match(str, re) )
			break;
		++(*this);
	}
}
}} // namespace
