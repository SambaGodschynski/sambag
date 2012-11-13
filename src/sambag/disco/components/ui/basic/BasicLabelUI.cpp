/*
 * BasicLabelUI.cpp
 *
 *  Created on: Tue Jun 12 17:23:39 2012
 *      Author: Johannes Unger
 */

#include "BasicLabelUI.hpp"
#include <sambag/disco/components/Label.hpp>
#include <sambag/disco/IDiscoFactory.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
namespace {
Rectangle calcIconBounds(ISurface::Ptr icon, const Coordinate &textHeight) {
	Rectangle res(0,0,textHeight,textHeight);
	return res;
}
void drawIcon(IDrawContext::Ptr cn, 
	ISurface::Ptr icon, 
	const Rectangle &bounds,
	AComponent::Ptr c) 
{
	cn->save();
	Rectangle realBounds = icon->getSize();
	if (realBounds.width() <= 0. || realBounds.height() <= 0.)
		return;
	sambag::com::Number scaleX = bounds.width() / realBounds.width();
	sambag::com::Number scaleY = bounds.height() / realBounds.height();
	cn->translate(Point2D(0, (c->getHeight() - bounds.height()) / 2.));
	cn->scale(Point2D(scaleX, scaleY));
	cn->drawSurface(icon);	
	cn->restore();
}
} // namespace(s)
//=============================================================================
//  Class BasicLabelUI
//=============================================================================
//-----------------------------------------------------------------------------
sambag::com::Number iconTextGap = 10.;
//-----------------------------------------------------------------------------
void BasicLabelUI::installUI(AComponentPtr c) {
	Super::installUI(c);
}
//-----------------------------------------------------------------------------
void BasicLabelUI::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	Label::Ptr l = boost::shared_dynamic_cast<Label>(c);
	cn->setFont(l->getFont());
	cn->setFillColor(c->getForeground());
	std::string txt = sambag::com::normString(l->getText());
	Rectangle txtEx = cn->textExtends(txt);
	ISurface::Ptr icon = l->getIcon();
	if (!icon) {
		cn->moveTo(Point2D(0,  c->getHeight() / 2.0 + txtEx.getHeight() / 2.0 ));
		cn->textPath(txt);
		cn->fill();
		return;
	}
	Rectangle iconBounds = calcIconBounds(icon, txtEx.getHeight());
	drawIcon(cn, icon, iconBounds, c);
	cn->moveTo(Point2D(iconBounds.width() + iconTextGap, 
		c->getHeight() / 2.0 + txtEx.getHeight() / 2.0));
	cn->textPath(txt);
	cn->fill();
}
//-----------------------------------------------------------------------------
Dimension BasicLabelUI::getPreferredSize(AComponentPtr c) {
	Label::Ptr l = boost::shared_dynamic_cast<Label>(c);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	// TODO: handle font style/size
	cn->setFont(l->getFont());
	Rectangle txtEx = cn->textExtends(sambag::com::normString(l->getText()));
	ISurface::Ptr icon = l->getIcon();
	Dimension res = Dimension(txtEx.getWidth() + 35, txtEx.getHeight() + 20);
	if (!icon)
		return res;
	Rectangle iconBounds = calcIconBounds(icon, txtEx.getHeight());
	res.width( res.width() + iconBounds.width() + iconTextGap);
	return res;
}

}}}}} // namespace(s)
