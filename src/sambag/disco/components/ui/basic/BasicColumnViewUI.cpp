/*
 * BasicColumnViewUI.cpp
 *
 *  Created on: Mon Sep 17 17:39:33 2012
 *      Author: Johannes Unger
 */

#include "BasicColumnViewUI.hpp"
#include <sambag/disco/components/ui/UIManager.hpp>

namespace sambag { namespace disco { namespace components {
namespace ui { namespace basic {
//=============================================================================
//  Class BasicColumnViewUI
//=============================================================================
//-----------------------------------------------------------------------------
BasicColumnViewUI::BasicColumnViewUI() {
}
//-----------------------------------------------------------------------------
void BasicColumnViewUI::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	/*cn->setFillColor(c->getBackground());
	cn->rect(cn->clipExtends());
	cn->fill();*/
}
//-----------------------------------------------------------------------------
void BasicColumnViewUI::installDefaults(AComponentPtr c) {
	UIManager &m = getUIManager();
	ColorRGBA bg(0,0,0);
	ColorRGBA fg(0,0,0);
	m.getProperty("ColumnView.background", bg);
	m.getProperty("ColumnView.foreground", fg);
	c->setBackground(bg);
	c->setForeground(fg);
}
//-----------------------------------------------------------------------------
void BasicColumnViewUI::installUI(AComponentPtr c) {
	installDefaults(c);
}
}}}}} // namespace(s)
