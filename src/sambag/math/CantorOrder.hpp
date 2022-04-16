/*
 * CantorOrder.hpp
 *
 *  Created on: Wed Oct 31 10:08:17 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CANTORORDER_H
#define SAMBAG_CANTORORDER_H

#include <boost/shared_ptr.hpp>
//#include "MetaSqrt.hpp"

namespace sambag {  namespace math { 
/**
 * template meta implementation of cantor ordering with a degree 
 * of N. (N is limited to MAX_N)
 *
 * Cantor<c1, c2, ... , cN> = x.
 * CantorInv<x, cn> = cx.
 *
 * Useful to flatten N-dimension arrays:
 *
 * int[MAX] arr2D;
 * arr[ Cantor<0,0> ] = 0;
 *
 * int[MAX] arr3D;
 * arr[ Cantor<0, 0, 0> ] = 0;
 */

}} // namespace(s)

#endif /* SAMBAG_CANTORORDER_H */
