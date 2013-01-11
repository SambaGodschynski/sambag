/*
 * BasicLabelUI.cpp
 *
 *  Created on: Tue Jun 12 17:23:39 2012
 *      Author: Johannes Unger
 */

#include "BasicLabelUI.hpp"
#include <sambag/disco/components/Label.hpp>
#include <sambag/disco/IDiscoFactory.hpp>

#include <sambag/disco/genFormatter/GenFlowLayout.hpp>
#include <sambag/disco/genFormatter/RectangleAccess.hpp>
#include <sambag/disco/genFormatter/GenericFormatter.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
namespace {
static const sambag::com::Number GAP= 5.;
Rectangle calcIconBounds(ISurface::Ptr icon, const Coordinate &textHeight) {
	if (!icon) {
		return Rectangle(0,0,0,0);	
	}
	Rectangle res(0,0,textHeight,textHeight);
	return res;
}
void drawIcon(IDrawContext::Ptr cn,
	ISurface::Ptr icon, 
	const Rectangle &bounds,
	AComponent::Ptr c) 
{
	if (!icon) {
		return;
	}
	cn->save();
	Rectangle realBounds = icon->getSize();
	if (realBounds.width() <= 0. || realBounds.height() <= 0.)
		return;
	sambag::com::Number scaleX = bounds.width() / realBounds.width();
	sambag::com::Number scaleY = bounds.height() / realBounds.height();
	cn->translate(bounds.x0());
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

	using namespace sambag::disco::genFormatter;
	Label::Ptr l = boost::shared_dynamic_cast<Label>(c);
	cn->setFont(l->getFont());
	cn->setFillColor(c->getForeground());
	std::string txt = sambag::com::normString(l->getText());
	Rectangle txtEx = cn->textExtends(txt);
	
	ISurface::Ptr icon = l->getIcon();
	Rectangle iconBounds = calcIconBounds(icon, txtEx.getHeight());
	
	typedef GenericFormatter< Rectangle,
		RectangleAccess,
		GenFlowLayout
	> Formatter;
	Formatter form;
	form.setHgap(GAP);
	form.setVgap(GAP);
	form.setAlignment(Formatter::CENTER);
	form.setWidth(c->getWidth());
	form.setHeight(c->getHeight());
	form.addComponent(&iconBounds);	
	form.addComponent(&txtEx);	
	form.layout();

	drawIcon(cn, icon, iconBounds, c);
	cn->moveTo(Point2D(
		txtEx.x(), txtEx.y() + txtEx.height()
	));
	cn->textPath(txt);
	cn->fill();
}
//-----------------------------------------------------------------------------
Dimension BasicLabelUI::getPreferredSize(AComponentPtr c) {
	
	Dimension res = getMinimumSize(c);
	res.width(res.width() + GAP*2.5);
	res.height(res.height() + GAP*2.5);
	return res;
}
//-----------------------------------------------------------------------------
Dimension BasicLabelUI::getMinimumSize(AComponentPtr c) {
	Label::Ptr l = boost::shared_dynamic_cast<Label>(c);
	if (!l) {
		return Dimension();
	}
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	cn->setFont(l->getFont());
	Rectangle txtEx = cn->textExtends(sambag::com::normString(l->getText()));
	Dimension res(txtEx.width(), txtEx.height());
	ISurface::Ptr icon = l->getIcon();
	if (!icon)
		return res;
	Rectangle iconBounds = calcIconBounds(icon, txtEx.getHeight());
	res.width( res.width() + iconBounds.width() + GAP*5.5);
	return res;

}

}}}}} // namespace(s)
