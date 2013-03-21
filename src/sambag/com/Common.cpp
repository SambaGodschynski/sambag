/*
 * Common.cpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#include "Common.hpp"
#include <iostream>
#include <boost/locale.hpp>

namespace sambag { namespace com {
//=============================================================================
//-----------------------------------------------------------------------------
void log(const std::string &str) {
	SAMBAG_LOG_INFO<<str;
}
//-----------------------------------------------------------------------------
std::string normString(const std::string &v) {
	return boost::locale::conv::to_utf<char>(v,"utf-8");
}
}}
