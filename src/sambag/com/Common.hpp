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

namespace sambag { namespace com {
using namespace boost;
//#############################################################################
// metrics
//#############################################################################
typedef double Number;
extern const Number NULL_NUMBER;
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

}} // namespaces

#endif /* COMMON_HPP_ */
