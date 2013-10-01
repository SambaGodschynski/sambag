/*
 * Common.cpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#include "Common.hpp"
#include <iostream>
#include <boost/locale.hpp>

#ifdef SAMBAG_USE_LOG
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#endif // SAMBAG_USE_LOG

namespace sambag { namespace com {
//=============================================================================
//-----------------------------------------------------------------------------
void log(const std::string &str) {
	SAMBAG_LOG_INFO<<str;
}
//-----------------------------------------------------------------------------
extern void addLogFile(const std::string &filename) {
#ifndef SAMBAG_USE_LOG
    return;
#endif // !SAMBAG_USE_LOG
    namespace logging = boost::log;
    namespace keywords = boost::log::keywords;
    
    logging::add_file_log (
        keywords::file_name = filename,
        //keywords::rotation_size = 10 * 1024 * 1024,
        keywords::format = "[%TimeStamp%]: %Message%"
    );
    logging::add_common_attributes();
}
//-----------------------------------------------------------------------------
std::string normString(const std::string &v) {
	return v; //TODO: linking fails MACOSX boost::locale::conv::to_utf<char>(v,"utf-8");
}
}}
