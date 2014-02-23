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
    if ( context->isFilled() ) {
        shape(context);
		context->fill();
	}
	if ( context->isStroked() ) {
		shape(context);
		context->stroke();
	}
}

}} // namespace(s)
