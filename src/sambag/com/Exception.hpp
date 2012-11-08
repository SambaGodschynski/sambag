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
#include <exception>

// TODO: remove string conv.
#define SAMBAG_THROW(type, what) {		\
		throw type( (what), std::string(__FILE__), \
			sambag::com::toString(__LINE__) ); \
	}

#define SAMBAG_EXCEPTION_CLASS(name) \
	struct name : public sambag::com::Exception { \
		name(const std::string &what = "unknown reason", \
			 const std::string &where = "unknown location", \
			 const std::string &line = "unknown line number") \
		 : sambag::com::Exception(what, where, line) {}\
	} \

#define SAMBAG_DERIVATED_EXCEPTION_CLASS(base, name) \
	struct name : public base { \
		name(const std::string &what = "unknown reason", \
			 const std::string &where = "unknown location", \
			 const std::string &line = "unknown line number") \
		 : sambag::com::Exception(what, where, line) {}\
	} \

#define SAMBAG_ASSERT(x) assert((x))

namespace sambag { namespace com {
//=============================================================================
struct Exception : public std::exception {
//=============================================================================
	//-------------------------------------------------------------------------
	std::string line;
	std::string where;
	std::string _what;
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
	: line(line), where(where), _what(what) {}
	//-------------------------------------------------------------------------
	virtual const char* what() const throw();
	//-------------------------------------------------------------------------
	virtual ~Exception() throw()
	{
	}
};
//-----------------------------------------------------------------------------
inline void warn(const std::string &msg) {
	log("warning: " + msg);
}
}}
#endif /* EXCEPTION_HPP_ */
