/*
 * UnitEx.cpp
 *
 *  Created on: Sun Nov 24 15:49:08 2013
 *      Author: Johannes Unger
 */

#include "UnitEx.hpp"
#include <cstring>
namespace sambag { namespace com { namespace unitex {

bool ignoreKnownIssues = false;
///////////////////////////////////////////////////////////////////////////////

void processArguments(int argc, char **argv) {
    for (int i=0; i<argc; ++i) {
        // quick and dirty, for more complex arguments use boost programm options
        if ( strcmp(argv[i], "--ignore-known-issues")==0 ) {
            ignoreKnownIssues=true;
        }
    }
}




}}} // namespace(s)
