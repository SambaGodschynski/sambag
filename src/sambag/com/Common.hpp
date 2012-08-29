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

#define SAMBA_LOG_NOT_YET_IMPL() {								\
		std::stringstream ss;									\
		ss<<__FUNCTION__<< " not yet implemented."<<std::endl;	\
		sambag::com::log(ss.str());								\
	}

namespace sambag { namespace com {
using namespace boost;
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
using namespace boost::logic;
typedef tribool TriBool;
//=============================================================================
extern void log(const std::string &str);
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
}} // namespaces

#endif /* COMMON_HPP_ */
