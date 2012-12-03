/*
 * TitledBorder.cpp
 *
 *  Created on: Sun Dec  2 12:56:15 2012
 *      Author: Johannes Unger
 */

#include "TitledBorder.hpp"
#include "SolidBorder.hpp"
#include "ui/ALookAndFeel.hpp"
#include <sambag/disco/components/AComponent.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class TitledBorder
//=============================================================================
//-----------------------------------------------------------------------------
TitledBorder::TitledBorder() {
	style.fillColor(Style::NONE_COLOR);
	style.strokeColor(ColorRGBA(0));
	style.strokeWidth(2.);
    Font f;
	f.size = 12.;
    style.font(f);
}
//-----------------------------------------------------------------------------
Point2D TitledBorder::getTextPos(AComponentPtr c, const Rectangle &size) const 
{
	return Point2D(10., 8.);
}
//-----------------------------------------------------------------------------
std::string TitledBorder::getText(AComponentPtr c, const Rectangle &size) const 
{
	return c ? c->getName() : "";
}
//-----------------------------------------------------------------------------
Rectangle TitledBorder::getTextBounds(AComponentPtr c,
    IDrawContext::Ptr cn, const Rectangle &r)
{
	cn->moveTo( getTextPos(c, r) );
    Rectangle res = cn->textExtends(getText(c, r));
    return res;
}
//-----------------------------------------------------------------------------
Insets TitledBorder::getBorderInsets(AComponentPtr c) {
	return getBorder()->getBorderInsets(c);
}
//-----------------------------------------------------------------------------
bool TitledBorder::isBorderOpaque() {
	return getBorder()->isBorderOpaque();
}
//-----------------------------------------------------------------------------
void TitledBorder::paintBorder(AComponentPtr c,IDrawContext::Ptr cn,
    const Rectangle &r)
{
	style.intoContext(cn);
    Rectangle textBounds = getTextBounds(c, cn, r);
    cn->save();
	cn->rect(r);
	cn->rect(textBounds);
	cn->setFillRule(IDrawContext::FILL_RULE_EVEN_ODD);
	cn->clip();
	
	Rectangle borderb = r;
	borderb.inset(-5., -5.);
	getBorder()->paintBorder(c, cn, borderb);
  
	cn->restore();
    cn->moveTo(getTextPos(c, r));
    cn->textPath(getText(c, r));
    cn->setFillColor(ColorRGBA(0));
    cn->fill();
}
//-----------------------------------------------------------------------------
void TitledBorder::setBorder(IBorder::Ptr border) {
	this->border = border;
}
//-----------------------------------------------------------------------------
IBorder::Ptr TitledBorder::getBorder() {
	if (!border) {
		border = SolidBorder::create();
	}
	return border;
}
//-----------------------------------------------------------------------------
void TitledBorder::setStyle(const Style &style) {
	this->style = style;
}
}}} // namespace(s)
