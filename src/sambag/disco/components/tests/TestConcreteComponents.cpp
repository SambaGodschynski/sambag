/*
 * TestConcreteComponents.cpp
 *
 *  Created on: Tue Jun  5 07:30:24 2012
 *      Author: Johannes Unger
 */

#include "TestConcreteComponents.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/ISurface.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <sambag/disco/components/ui/basic/BasicLookAndFeel.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/tests/HelperForTesting.hpp>
#include <boost/filesystem.hpp>
#include <sambag/disco/components/events/MouseEventCreator.hpp>
#include "TestComponents.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestConcreteComponents );

namespace tests {
//=============================================================================
//  Class TestConcreteComponents
//=============================================================================
//-----------------------------------------------------------------------------
void TestConcreteComponents::setUp() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	IDiscoFactory *fac = getDiscoFactory();
	surf = fac->createImageSurface(320, 80);
	root = RootPane::create(surf);
	ui::UIManager::instance().installLookAndFeel(
			root,
			ui::basic::BasicLookAndFeel::create()
	);
	root->setSize(320, 80);
	root->validate();
}
//-----------------------------------------------------------------------------
void TestConcreteComponents::testButton() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	using namespace sambag::disco::components::events;
	using namespace boost;
	MouseEventCreator::Ptr evc = MouseEventCreator::create();
    evc->setRootPane(root);
	tests::TestButtonAction action;
	Button::Ptr btn = Button::create();
	btn->setText("do it!");
	btn->setName("Button01");
	btn->setButtonFunction(
		boost::bind(&tests::TestButtonAction::execute, &action)
	);
	root->add(btn);
	root->validate();
	root->draw( root->getDrawContext() );
	surf->writeToFile(OUTPUT_FOLDER + "/testConcreteButton.png");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<rollover
	evc->createMoveEvent(159,17);
	root->draw( root->getDrawContext() );
	surf->writeToFile(OUTPUT_FOLDER + "/testConcreteButton(ROLLOVER).png");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<click
	evc->createPressEvent(159,17,1);
	root->draw( root->getDrawContext() );
	surf->writeToFile(OUTPUT_FOLDER + "/testConcreteButton(PRESSED).png");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<release
	evc->createReleaseEvent(159,17,1);
	evc->createMoveEvent(0,0);
	root->draw( root->getDrawContext() );
	surf->writeToFile(OUTPUT_FOLDER + "/testConcreteButton(RELEASED).png");
	CPPUNIT_ASSERT(action.wasCalled());
}
} //namespace
