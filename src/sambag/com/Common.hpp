/*
 * Common.hpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#ifndef SAMBAG_COMMON_HPP_
#define SAMBAG_COMMON_HPP_

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
// Math
//#############################################################################
//=============================================================================
typedef boost::numeric::ublas::matrix<Number> Matrix;
extern const Matrix IDENTITY_MATRIX;
extern const Matrix NULL_MATRIX;
//=============================================================================
using namespace boost::numeric::ublas;
//=============================================================================
// Missing comparator in Matrix.hpp
template<typename T>
bool operator==(const matrix<T>& m, const matrix<T>& n) {
//=============================================================================
	bool returnValue = (m.size1() == n.size1()) && (m.size2() == n.size2());
	if (returnValue) {
		for (unsigned int i = 0; returnValue && i < m.size1(); ++i) {
			for (unsigned int j = 0; returnValue && j < m.size2(); ++j) {
				returnValue &= m(i, j) == n(i, j);
			}
		}
	}
	return returnValue;
}
//=============================================================================
template<typename T>
bool operator!=(const matrix<T>& m, const matrix<T>& n) {
//=============================================================================
	return !(m == n);
}
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
