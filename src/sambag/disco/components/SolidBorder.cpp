/*
 * SolidBorder.cpp
 *
 *  Created on: Sun Dec  2 11:07:33 2012
 *      Author: Johannes Unger
 */

#include "SolidBorder.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class SolidBorder
//=============================================================================
//-----------------------------------------------------------------------------
SolidBorder::SolidBorder() {
	style.fillColor(Style::NONE_COLOR);
	style.strokeColor(ColorRGBA(0));
	style.strokeWidth(2.);
    Font f;
    style.font(f);
}
//-----------------------------------------------------------------------------
Insets SolidBorder::getBorderInsets(AComponentPtr c) {
	return Insets(0,0,0,0);
}
//-----------------------------------------------------------------------------
void SolidBorder::setCornerRadius(const Coordinate &c) {
	cornerRadius = c;
}
//-----------------------------------------------------------------------------
bool SolidBorder::isBorderOpaque() {
	return true;
}
//-----------------------------------------------------------------------------
void SolidBorder::paintBorder(AComponentPtr c,IDrawContext::Ptr cn, 
	const Rectangle &r)
{
	style.intoContext(cn);
	Number ins = cn->getStrokeWidth();
	Rectangle rect = r;
	rect.inset(-ins,-ins);
	cn->rect(rect, cornerRadius);
	cn->stroke();
}
//-----------------------------------------------------------------------------
void SolidBorder::setStyle(const Style &style) {
	this->style = style;
}
}}} // namespace(s)
