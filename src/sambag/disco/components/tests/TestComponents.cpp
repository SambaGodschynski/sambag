/*
 * TestComponents.cpp
 *
 *  Created on: May 22, 2012
 *      Author: samba
 */

#include "TestComponents.hpp"
#include <sstream>

namespace tests {
//=============================================================================
//-----------------------------------------------------------------------------
int TestComponent::numDrawCalled = 0;
//-----------------------------------------------------------------------------
void TestComponent::drawComponent(sambag::disco::IDrawContext::Ptr cn) {
	using namespace sambag::disco;
	using namespace sambag::com;
	cn->setFillColor(getBackground());
	cn->rect(Rectangle(0,0,getWidth(), getHeight()));
	cn->fill();
	numDrawCalled = numDrawCalled + 1;
	// draw index if exists
	int index = -1;
	get( getClientProperty("index"), index );
	if (index<0)
		return;
	std::stringstream ss;
	ss<<index;
	std::string str = ss.str();
	cn->setFontSize(22);
	cn->setFillColor(ColorRGBA(1,1,1));
	Rectangle txt = cn->textExtends(str);
	cn->translate( Point2D( getWidth() / 2.0 - txt.getWidth() / 2.0,
			getHeight() / 2.0 + txt.getHeight() / 2.0
	));
	cn->textPath(str);
	cn->fill();
}
//-----------------------------------------------------------------------------
LookAndFeelForTest::LookAndFeelForTest() {
	registerComponentUI<TestComponent, TestComponentUI>();
}
//-----------------------------------------------------------------------------
sdc::ui::AComponentUIPtr
TestComponent::createComponentUI(sdc::ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<TestComponent>();
}
}
