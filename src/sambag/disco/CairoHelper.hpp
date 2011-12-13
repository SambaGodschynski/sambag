/*
 * CairoHelper.hpp
 *
 *  Created on: Dec 15, 2011
 *      Author: samba
 */

#ifndef CAIROHELPER_HPP_
#define CAIROHELPER_HPP_

#include <cairo.h>
#include "sambag/math/Matrix.hpp"

namespace sambag { namespace disco {
using namespace sambag::math;
//-----------------------------------------------------------------------------
/**
 * assumes a 3x3 src. matrix
 * @param dM
 * @param cM
 */
inline void discoMatrixToCairoMatrix ( const Matrix &dM, cairo_matrix_t &cM ) {
	if ( dM.size1() != 3 || dM.size2() != 3 ) return;
	cM.xx = dM(0,0); cM.yx = dM(1,0);
	cM.xy = dM(0,1); cM.yy = dM(1,1);
	cM.x0 = dM(0,2); cM.y0 = dM(1,2);
}
//-----------------------------------------------------------------------------
/**
 * assumes a 3x3 dst. matrix
 * @param cM
 * @param dM
 */
inline void cairoMatrixToDiscoMatrix ( const cairo_matrix_t &cM, Matrix &dM ) {
	if ( dM.size1() != 3 || dM.size2() != 3 ) return;
	dM(0,0) = cM.xx; dM(0,1) = cM.xy; dM(0,2) = cM.x0;
	dM(1,0) = cM.yx; dM(1,1) = cM.yy; dM(1,2) = cM.y0;
	dM(2,0) = 0;     dM(2,1) = 0    ; dM(2,2) = 0;
}

}}

#endif /* CAIROHELPER_HPP_ */
