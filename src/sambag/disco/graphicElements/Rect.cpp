/*
 * Rect.cpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#include "Rect.hpp"
#include "sambag/com/Common.hpp"
namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
// class Rect
//=============================================================================
//-----------------------------------------------------------------------------
Rect::Rect(const Rectangle &rect) : rect(rect) {
}
//-----------------------------------------------------------------------------
Rect::~Rect() {
}
//-----------------------------------------------------------------------------
void Rect::draw( IDrawContext::Ptr cn ) {
	AutoTransform at(getTransformMatrix(), cn);
	if ( style.isFilled() ) {
		cn->rect(rect);
		style.setFillStyle(cn);
		cn->fill();
	}
	if ( style.isStroked() ) {
		cn->rect(rect);
		style.setStrokeStyle(cn);
		cn->stroke();
	}
}

}}}
