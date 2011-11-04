/*
 * Exception.hpp
 *
 *  Created on: Nov 30, 2011
 *      Author: samba
 */

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#define RAISE_ERROR(obj, what) throw obj((what), sambag::com::Exception::ERROR, std::string(__FILE__),  std::string(__LINE__))
#define RAISE_WARNING(obj, what) throw obj((what), sambag::com::Exception::WARNING)
#define RAISE_CRITICAL(obj, what) throw obj((what), sambag::com::Exception::CRITICAL,  std::string(__FILE__), std::string( __LINE__)

#define EXCEPTION_CLASS(name) \
	struct name : public sambag::com::Exception { \
		name(const std::string &what = "unknown reason", \
			 sambag::com::Exception::Type type = sambag::com::Exception::UNKOWN, \
			 const std::string &where = "unknown location", \
			 const std::string &line = "unknown line number") \
		 : sambag::com::Exception(what, type, where, line) {}\
	} \

#include <string>

namespace sambag { namespace com {
//=============================================================================
class Exception {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	enum Type {UNKOWN, WARNING, ERROR, CRITICAL};
private:
	std::string line;
	std::string where;
	std::string what;
	Type type;
public:
	//-------------------------------------------------------------------------
	Exception(const std::string &what = "unknown reason",
			  Type type = UNKOWN,
			  const std::string &where = "unknown location",
			  const std::string &line = "unknown line number")
	: line(line), where(where), what(what), type(type) {}
	//-------------------------------------------------------------------------
	virtual ~Exception() {}
};
}}
#endif /* EXCEPTION_HPP_ */
