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
void Text::draw( IDrawContext::Ptr cn ) {
	cn->setFont( style.font() );
	if ( style.isFilled() ) {
		cn->moveTo(pos);
		cn->textPath(text);
		style.setFillStyle(cn);
		cn->fill();
	}
	if ( style.isStroked() ) {
		cn->moveTo(pos);
		cn->textPath(text);
		style.setStrokeStyle(cn);
		cn->stroke();
	}
}

}}}
