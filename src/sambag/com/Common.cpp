/*
 * Common.cpp
 *
 *  Created on: 13.09.2011
 *      Author: samba
 */

#include "Common.hpp"
#include <float.h>

namespace sambag { namespace com {

const Point2D   NULL_POINT2D(0,0);
const Rectangle NULL_RECTANGLE;
const ColorRGBA NULL_COLOR = ColorRGBA(NULL_NUMBER,NULL_NUMBER,NULL_NUMBER,NULL_NUMBER);
const ColorRGBA NONE_COLOR = ColorRGBA(-1,-1,-1,-1);
const Number NULL_NUMBER = DBL_MAX;
const Matrix IDENTITY_MATRIX = identity_matrix<Number> (3,3);

}}
