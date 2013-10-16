/*
 * Common.hpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#ifndef SAMBAG_COMMON_HPP_
#define SAMBAG_COMMON_HPP_

// common used headers
#include <sambag/math/VectorN.hpp>
#include <math.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <float.h>
#include <limits.h>
#include <limits>
#include <string>
#include <sstream>

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// BEGIN LOG STUFF

struct __LogDummy_ {
        template <typename T>
        __LogDummy_ & operator << (const T & t) { return *this; }
};

#ifdef SAMBAG_USE_LOG
#include <boost/log/trivial.hpp>
#define SAMBAG_LOG_WARN_IMPL  BOOST_LOG_TRIVIAL(warning)
#define SAMBAG_LOG_ERR_IMPL   BOOST_LOG_TRIVIAL(error)
#define SAMBAG_LOG_INFO_IMPL  BOOST_LOG_TRIVIAL(info)
#define SAMBAG_LOG_DEBUG_IMPL BOOST_LOG_TRIVIAL(debug)
#define SAMBAG_LOG_TRACE_IMPL BOOST_LOG_TRIVIAL(trace)
#define SAMBAG_LOG_FATAL_IMPL BOOST_LOG_TRIVIAL(fatal)
#else
#define SAMBAG_LOG_WARN_IMPL  __LogDummy_()
#define SAMBAG_LOG_ERR_IMPL   __LogDummy_()
#define SAMBAG_LOG_INFO_IMPL  __LogDummy_()
#define SAMBAG_LOG_DEBUG_IMPL __LogDummy_()
#define SAMBAG_LOG_TRACE_IMPL __LogDummy_()
#define SAMBAG_LOG_FATAL_IMPL __LogDummy_()
#endif //SAMBAG_USE_LOG

#define SAMBAG_LOG_WARN  SAMBAG_LOG_WARN_IMPL 
#define SAMBAG_LOG_ERR   SAMBAG_LOG_ERR_IMPL
#define SAMBAG_LOG_INFO  SAMBAG_LOG_INFO_IMPL 
#define SAMBAG_LOG_DEBUG SAMBAG_LOG_DEBUG_IMPL 
#define SAMBAG_LOG_TRACE SAMBAG_LOG_TRACE_IMPL
#define SAMBAG_LOG_FATAL SAMBAG_LOG_FATAL_IMPL
/**
 * @deprecated use SAMBAG_LOG_WARN<<msg
 */
#define SAMBAG_WARN(msg) SAMBAG_LOG_WARN<<msg

#ifdef SAMBA_WARN_MISSING_IMPL
	#define SAMBA_LOG_NOT_YET_IMPL()								\
		SAMBAG_LOG_INFO<<__FUNCTION__<< " not yet implemented.";
#else 
#define SAMBA_LOG_NOT_YET_IMPL()
#endif

// END LOG STUFF
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace sambag { namespace com {
//#############################################################################
// metrics
//#############################################################################
typedef int Integer;
typedef double Number;
#define NULL_INTEGER sambag::com::getNullInteger()
#define NULL_NUMBER sambag::com::getNullNumber()
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//#############################################################################
// Misc.
//#############################################################################
//=============================================================================
#define boost_for_each BOOST_FOREACH
#define boost_reverse_for_each BOOST_REVERSE_FOREACH
//=============================================================================
typedef boost::logic::tribool TriBool;
//=============================================================================
/**
 * @deprecated: use SAMBAG_LOG_XXX macro.
 */
extern void log(const std::string &str);
//-----------------------------------------------------------------------------
extern void addLogFile(const std::string &filename);
//-----------------------------------------------------------------------------
inline Integer getNullInteger() {
	return INT_MAX;
}
//-----------------------------------------------------------------------------
inline Number getNullNumber() {
	BOOST_STATIC_ASSERT( boost::is_floating_point<Number>::value );
	return std::numeric_limits<Number>::infinity(); //NaN occurs problems
}
//-----------------------------------------------------------------------------
template<typename T>
std::string toString(const T &v) {
	std::stringstream ss;
	ss << v;
	return ss.str();
}
//-----------------------------------------------------------------------------
extern std::string normString(const std::string &v);
}} // namespaces

#endif /* COMMON_HPP_ */
