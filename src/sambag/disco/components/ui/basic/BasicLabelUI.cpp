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
	cn->moveTo(Point2D(0,  c->getHeight() / 2.0 + txtEx.getHeight() / 2.0 ));
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
	return Dimension(txtEx.getWidth() + 35, txtEx.getHeight() + 15);
}

}}}}} // namespace(s)
