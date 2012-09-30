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
#ifdef SAMBAG_USE_LOG
	using namespace std;
	clog<<str<<endl;
#endif
}
//-----------------------------------------------------------------------------
std::string normString(const std::string &v) {
	return boost::locale::conv::to_utf<char>(v,"utf-8");
}
}}
