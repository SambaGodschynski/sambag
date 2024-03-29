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
#include <sambag/disco/components/FlowLayout.hpp>
#include <sambag/disco/tests/HelperForTesting.hpp>
#include <boost/filesystem.hpp>
#include <sambag/disco/components/events/MouseEventCreator.hpp>
#include "TestComponents.hpp"
#include <sambag/disco/components/WindowToolkit.hpp>

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
	namespace sd = sambag::disco;
	if (!wfac) {
		wfac = WindowTestToolkit<>::create();
	}
	sambag::disco::components::setWindowToolkit(wfac.get());

	win = Window::create();

	win->setWindowBounds(sd::Rectangle(0,0, 320, 80));

	surf = getDiscoFactory()->createImageSurface(320, 80);
	root = win->getRootPane();
	root->setSurface(surf);
	cont = win->getContentPane();
	cont->setLayout(FlowLayout::create());
	cont->setPreferredSize(Dimension(320, 80));
	win->revalidate();
	win->open();
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
	cont->add(btn);
	root->revalidate();
	
	std::stringstream ss;
	root->printComponentTree(ss);

	/*CPPUNIT_ASSERT_EQUAL(std::string("[RootPane,0,0,320x80,alignmentX=0.5,alignmentY=0.5]\
 [RootPane conetent pane,135,17,320x280,invalid,alignmentX=0.5\
,alignmentY=0.5]\
 [do it!, Button01,140,5,44.9626x24,invalid]"), ss.str());*/

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
