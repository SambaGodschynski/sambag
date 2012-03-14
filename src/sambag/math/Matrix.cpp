/*
 * Matrix.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: samba
 */

#include "Matrix.hpp"

namespace sambag { namespace math {
namespace {
	using namespace sambag::com;
	Matrix createNullMatrix() {
		Matrix m(3,3);
		m(0,0) = NULL_NUMBER; m(0,1) = NULL_NUMBER; m(0,2) = NULL_NUMBER;
		m(1,0) = NULL_NUMBER; m(1,1) = NULL_NUMBER; m(1,2) = NULL_NUMBER;
		m(2,0) = NULL_NUMBER; m(2,1) = NULL_NUMBER; m(2,2) = NULL_NUMBER;
		return m;
	}
}
const Matrix IDENTITY_MATRIX = identity_matrix<sambag::com::Number> (3,3);
const Matrix NULL_MATRIX = createNullMatrix();

}}
