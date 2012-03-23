/*
 * Text.cpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#include "Text.hpp"
#include "sambag/com/Common.hpp"
namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
// class Text
//=============================================================================
//-----------------------------------------------------------------------------
Text::Text() : pos(Point2D(0,0)){
}
//-----------------------------------------------------------------------------
Text::~Text() {
}
//-----------------------------------------------------------------------------
void Text::drawPath(IDrawContext::Ptr cn) {
	if (storedPath) {
		cn->appendPath(storedPath);
		return;
	}
	cn->textPath(text);
	storedPath = cn->copyPath();

}
//-----------------------------------------------------------------------------
void Text::draw( IDrawContext::Ptr cn ) {
	if ( cn->isFilled() ) {
		cn->moveTo(pos);
		drawPath(cn);
		cn->fill();
	}
	if ( cn->isStroked() ) {
		cn->moveTo(pos);
		drawPath(cn);
		cn->stroke();
	}
}

}}}
