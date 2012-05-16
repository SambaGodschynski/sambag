/*
 * Common.cpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#include "Common.hpp"
#include <float.h>
#include <limits.h>
#include <iostream>
namespace sambag { namespace com {

const Number NULL_NUMBER = DBL_MAX;
const Integer NULL_INTEGER = INT_MAX;
//=============================================================================
//-----------------------------------------------------------------------------
void log(const std::string &str) {
	using namespace std;
	clog<<str<<endl;
}

}}
