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
	return ss.str();
}
}}
