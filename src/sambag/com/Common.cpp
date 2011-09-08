/*
 * Common.cpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#include "Common.hpp"
#include <float.h>

namespace sambag { namespace com {
namespace {
	Matrix createNullMatrix() {
		Matrix m(3,3);
		m(0,0) = NULL_NUMBER; m(0,1) = NULL_NUMBER; m(0,2) = NULL_NUMBER;
		m(1,0) = NULL_NUMBER; m(1,1) = NULL_NUMBER; m(1,2) = NULL_NUMBER;
		m(2,0) = NULL_NUMBER; m(2,1) = NULL_NUMBER; m(2,2) = NULL_NUMBER;
		return m;
	}
}
const Point2D   NULL_POINT2D(0,0);
const Rectangle NULL_RECTANGLE;
const ColorRGBA NULL_COLOR = ColorRGBA(NULL_NUMBER,NULL_NUMBER,NULL_NUMBER,NULL_NUMBER);
const ColorRGBA NONE_COLOR = ColorRGBA(-1,-1,-1,-1);
const Number NULL_NUMBER = DBL_MAX;
const Matrix IDENTITY_MATRIX = identity_matrix<Number> (3,3);
const Matrix NULL_MATRIX = createNullMatrix();

}}
