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
static const sambag::com::Number GAP= 5.;
Rectangle calcIconBounds(ISurface::Ptr icon, const Coordinate &textHeight) {
	Rectangle res(0,0,textHeight,textHeight);
	return res;
}
void drawIcon(IDrawContext::Ptr cn, // TODO: impl. and use glyph renderer for purpose
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
	cn->translate(Point2D(
		iconPadding, 
		(c->getHeight() - bounds.height()) / 2.
	));
	cn->scale(Point2D(scaleX, scaleY));
	cn->drawSurface(icon);	
	cn->restore();
}
} // namespace(s)
//=============================================================================
//  Class BasicLabelUI
//=============================================================================
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
	Rectangle iconBounds = calcIconBounds(icon, l->getFont().size);
	drawIcon(cn, icon, iconBounds, c);
	cn->moveTo(Point2D(iconBounds.width() + iconPadding*2., 
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
	std::string txt = sambag::com::normString(l->getText());
	/*if (txt == "")
		txt = "0"; // dummy*/
	Rectangle txtEx = cn->textExtends(txt);
	ISurface::Ptr icon = l->getIcon();
	Dimension res = Dimension(txtEx.getWidth() + GAP,
		txtEx.getHeight() + GAP );
	if (!icon)
		return res;
	Rectangle iconBounds = calcIconBounds(icon, l->getFont().size);
	res.width( res.width() + iconBounds.width() + GAP*2.);
	return res;
}
//-----------------------------------------------------------------------------
Dimension BasicLabelUI::getMinimumSize(AComponentPtr c) {
	Label::Ptr l = boost::shared_dynamic_cast<Label>(c);
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	// TODO: handle font style/size
	cn->setFont(l->getFont());
	Rectangle txtEx = cn->textExtends(sambag::com::normString(l->getText()));
	Dimension res(txtEx.width(), txtEx.height());
	ISurface::Ptr icon = l->getIcon();
	if (!icon)
		return res;
	Rectangle iconBounds = calcIconBounds(icon, l->getFont().size);
	res.width( res.width() + iconBounds.width() + GAP*2.);
	return res;

}

}}}}} // namespace(s)
