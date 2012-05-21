/*
 * Common.cpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#include "Common.hpp"
#include <iostream>
namespace sambag { namespace com {

//=============================================================================
//-----------------------------------------------------------------------------
void log(const std::string &str) {
	using namespace std;
	clog<<str<<endl;
}

}}
