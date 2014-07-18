/*
 * Shape.cpp
 *
 *  Created on: Sun Feb 23 10:21:25 2014
 *      Author: Johannes Unger
 */

#include "Shape.hpp"

namespace sambag { namespace disco {
//=============================================================================
//  Class Shape
//=============================================================================
//-----------------------------------------------------------------------------
void Shape::draw(IDrawContext::Ptr context) {
    bool filled = context->isFilled();
    bool stroked = context->isStroked();
    if (!filled && !stroked) {
        return;
    }
    shape(context);
    if (filled) {
        if (stroked) {
            context->fillPreserve();
        } else {
            context->fill();
        }
	}
	if (stroked) {
		context->stroke();
	}
}

}} // namespace(s)
