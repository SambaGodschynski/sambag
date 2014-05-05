/**
 * @author samba godschynski
 */

#ifndef SAMBAG_SVG_PATTERN_MATRIX
#define SAMBAG_SVG_PATTERN_MATRIX

#include <sambag/math/Matrix.hpp>


namespace sambag { namespace disco { namespace svg {
	    
struct SvgPatternMatrix : public Matrix {
    SvgPatternMatrix(){}
    SvgPatternMatrix(const Matrix &m) : Matrix(m) {}
    void operator=(const Matrix &m) {
	*this = SvgPatternMatrix(m);
    }
};

}}}

#endif //SAMBAG_SVG_PATTERN_MATRIX
