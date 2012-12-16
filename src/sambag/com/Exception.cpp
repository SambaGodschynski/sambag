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
Exception::Exception(const std::string &what, const std::string &where,
		  const std::string &line) : line(line), where(where), _what(what)
{
	log("Exception: " + what + ", " + where + ":" + line);
}
//-----------------------------------------------------------------------------
const char * Exception::what() const throw() {
	return _what.c_str();
}
}} //namespace(s)