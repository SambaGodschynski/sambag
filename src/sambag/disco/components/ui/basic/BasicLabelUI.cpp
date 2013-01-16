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
#include "Helper.hpp"

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
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
	
	Helper::drawIconAndText(cn, c->getIcon(), txt, 
		Rectangle(0, 0, c->getWidth(), c->getHeight()), false);
}
//-----------------------------------------------------------------------------
Dimension BasicLabelUI::getPreferredSize(AComponentPtr c) {
	
	Dimension res = getMinimumSize(c);
	res.width(res.width() + 25.5);
	res.height(res.height() + 25.5);
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
	Dimension res(txtEx.width(), cn->getCurrentFontHeight());
	ISurface::Ptr icon = l->getIcon();
	if (!icon)
		return res;
	Rectangle iconBounds = Helper::calcIconBounds(icon, res.height());
	res.width( res.width() + iconBounds.width() + 25.5);
	res.height( res.height() );
	return res;

}

}}}}} // namespace(s)
