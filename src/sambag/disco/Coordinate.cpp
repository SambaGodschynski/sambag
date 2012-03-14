/*
 * Coordinate.cpp
 *
 *  Created on: Dec 13, 2011
 *      Author: samba
 */

#include "Coordinate.hpp"
#include <sstream>

namespace sambag { namespace disco {
//=============================================================================
// struct Coordinate
//=============================================================================
//-----------------------------------------------------------------------------
std::string Coordinate::toString() const {
	std::stringstream ss;
	ss<<value;
	switch (type) {
	case PX:
		ss<<"px"; break;
	case IN:
		ss<<""; break;
	case CM:
		ss<<"cm"; break;
	case MM:
		ss<<"mm"; break;
	case PT:
		ss<<"pt"; break;
	case PC:
		ss<<"pc"; break;
	case EM:
		ss<<"em"; break;
	case EX:
		ss<<"ex"; break;
	case PERCENT:
		ss<<"%"; break;
	default:
		break;
	}
	return ss.str();
}
}}
