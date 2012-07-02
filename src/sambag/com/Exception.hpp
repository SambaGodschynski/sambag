/*
 * Exception.hpp
 *
 *  Created on: Nov 30, 2011
 *      Author: samba
 */


/* TODO: consider approach!!*/

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <string>
#include <iostream>
#include <assert.h>
#include "Common.hpp"

// TODO: remove string conv.
#define SAMBAG_THROW(type, what) {		\
		std::stringstream ss;				\
		ss<<__LINE__;						\
		throw type((what), std::string(__FILE__), ss.str()); \
	}
#define SAMBAG_WARN(what) sambag::com::warn((what))

#define SAMBAG_EXCEPTION_CLASS(name) \
	struct name : public sambag::com::Exception { \
		name(const std::string &what = "unknown reason", \
			 const std::string &where = "unknown location", \
			 const std::string &line = "unknown line number") \
		 : sambag::com::Exception(what, where, line) {}\
	} \

#define SAMBAG_ASSERT(x) assert((x))

namespace sambag { namespace com {
//=============================================================================
struct Exception {
//=============================================================================
	//-------------------------------------------------------------------------
	std::string line;
	std::string where;
	std::string what;
public:
	//-------------------------------------------------------------------------
	/**
	 * TODO change line argument type from string to int
	 * @param what
	 * @param where
	 * @param line
	 */
	Exception(const std::string &what = "unknown reason",
			  const std::string &where = "unknown location",
			  const std::string &line = "unknown line number")
	: line(line), where(where), what(what) {}
};
//-----------------------------------------------------------------------------
inline void warn(const std::string &msg) {
	log("warning: " + msg);
}
}}
#endif /* EXCEPTION_HPP_ */
