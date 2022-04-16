/*
 * UnitEx.cpp
 *
 *  Created on: Sun Nov 24 15:49:08 2013
 *      Author: Johannes Unger
 */

#include "UnitEx.hpp"
#include <cstring>

#ifdef SAMBAG_USE_LOG
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#endif

namespace sambag { namespace com { namespace unitex {
bool ignoreKnownIssues = false;
///////////////////////////////////////////////////////////////////////////////

void verbose() {
#if 0 //SAMBAG_USE_LOG fails to compile gcc-llvm
    namespace logging = boost::log;
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );
#endif
}

void startupUnitTests() {
#if 0 ///SAMBAG_USE_LOG fails to compile gcc-llvm
    namespace logging = boost::log;
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::error
    );
#endif
}
void processArguments(int argc, char **argv) {
    for (int i=0; i<argc; ++i) {
        // quick and dirty, for more complex arguments use boost programm options
        if ( strcmp(argv[i], "--ignore-known-issues")==0 ) {
            ignoreKnownIssues=true;
        }
        if ( strcmp(argv[i], "--verbose")==0 ) {
            verbose();
        }

    }
}




}}} // namespace(s)
