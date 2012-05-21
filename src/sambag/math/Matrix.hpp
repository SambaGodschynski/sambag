/*
 * Matrix.hpp
 *
 *  Created on: Dec 2, 2011
 *      Author: samba
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <math.h>
#include "sambag/com/Common.hpp"
#include <boost/numeric/ublas/matrix.hpp>

namespace sambag { namespace math {
using namespace boost;
//#############################################################################
// Matrix definitions
//#############################################################################
//=============================================================================
typedef boost::numeric::ublas::matrix<sambag::com::Number> Matrix;
#define IDENTITY_MATRIX identity_matrix<sambag::com::Number> (3,3)
#define NULL_MATRIX createNullMatrix()
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
//=============================================================================
inline Matrix translate2D(sambag::com::Number x, sambag::com::Number y) {
	Matrix tr = IDENTITY_MATRIX;
	tr(0,2) = x;
	tr(1,2) = y;
	return tr;
}
inline Matrix rotate2D(sambag::com::Number r) {
	using namespace sambag::com;
	Matrix m = IDENTITY_MATRIX;
	Number _cos = cos(r * M_PI / 180.0);
	Number _sin = sin(r * M_PI / 180.0);
	m(0,0) = _cos;
	m(1,0) = _sin;
	m(0,1) = -_sin;
	m(1,1) = _cos;
	return m;
}
inline Matrix rotate2D(sambag::com::Number r, sambag::com::Number x, sambag::com::Number y) {
	using namespace sambag::com;
	Matrix m = translate2D(x,y);
	m = boost::numeric::ublas::prod(m, rotate2D(r));
	m = boost::numeric::ublas::prod(m, translate2D(-x, -y));
	return m;
}
inline Matrix scale2D(sambag::com::Number x, sambag::com::Number y) {
	Matrix m = IDENTITY_MATRIX;
	m(0,0) = x;
	m(1,1) = y;
	return m;
}
inline Matrix skew2D(sambag::com::Number x, sambag::com::Number y) {
	using namespace sambag::com;
	Matrix m = IDENTITY_MATRIX;
	Number _tanX = tan(x * M_PI / 180.0);
	Number _tanY = tan(y * M_PI / 180.0);
	m(0,1) = _tanX;
	m(1,0) = _tanY;
	return m;
}
//-----------------------------------------------------------------------------
inline Matrix createNullMatrix() {
	Matrix m(3,3);
	m(0,0) = NULL_NUMBER; m(0,1) = NULL_NUMBER; m(0,2) = NULL_NUMBER;
	m(1,0) = NULL_NUMBER; m(1,1) = NULL_NUMBER; m(1,2) = NULL_NUMBER;
	m(2,0) = NULL_NUMBER; m(2,1) = NULL_NUMBER; m(2,2) = NULL_NUMBER;
	return m;
}
//-----------------------------------------------------------------------------

//=============================================================================
}} // namespaces

#endif /* MATRIX_HPP_ */
