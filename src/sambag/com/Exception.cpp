/*
 * Exception.cpp
 *
 *  Created on: Nov 30, 2011
 *      Author: samba
 */

#include "Exception.hpp"

namespace sambag { namespace com {
//=============================================================================
// struct Exception {
//=============================================================================
//-----------------------------------------------------------------------------
const char * Exception::what() const throw() {
	return _what.c_str();
}
}} //namespace(s)