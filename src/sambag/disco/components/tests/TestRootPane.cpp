/*
 * TestRootPane.cpp
 *
 *  Created on: Wed May 30 14:36:26 2012
 *      Author: Johannes Unger
 */

#include "TestRootPane.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/tests/HelperForTesting.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/events/MouseEventCreator.hpp>
#include <sambag/disco/components/ui/ALookAndFeel.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <sambag/disco/components/FlowLayout.hpp>
#include <boost/filesystem.hpp>
#include "TestComponents.hpp"
#include <stack>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestRootPane );

namespace tests {
//=============================================================================
//  Class TestRootPane
//=============================================================================
//-----------------------------------------------------------------------------
void TestRootPane::setUp() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	namespace sd = sambag::disco; // conflict with window.h Rectangle (why?)
	if (!wfac) {
		wfac = WindowTestToolkit<>::create();
	}
	sambag::disco::components::setWindowToolkit(wfac.get());

	win = Window::create();
	win->setBounds(0,0, 640, 480);
	win->open();

	surf = getDiscoFactory()->createImageSurface(640, 480);

	root = win->getRootPane();

	int i = 0;
	for (; i < NUM_COMPOS / 2; ++i) {
		std::stringstream ss;
		ss << "TestComponent" << i;
		comps[i] = TestComponent::create();
		comps[i]->setName(ss.str());
		comps[i]->setBounds(sd::Rectangle(0,0,50 + i*10, 55 + i*10));
		comps[i]->setBackground(ColorRGBA(0,0,0,1));
		root->add(comps[i]);
	}
	TestContainer::Ptr con = TestContainer::create();
	con->setLayout(FlowLayout::create());
	con->setPreferredSize(Dimension(150, 100));
	root->add(con);
	for (; i < NUM_COMPOS; ++i) {
		std::stringstream ss;
		ss << "TestComponent" << i;
		comps[i] = TestComponent::create();
		comps[i]->setName(ss.str());
		comps[i]->setBounds(sd::Rectangle(0,0,30, 30));
		comps[i]->setBackground(ColorRGBA(0,0,0,1));
		con->add(comps[i]);
	}
	root->validate();
}
//-----------------------------------------------------------------------------
void TestRootPane::testRepaint() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	TestComponent::numDrawCalled = 0;
	comps[0]->redraw();
	comps[1]->redraw();
	comps[2]->redraw();
	root->redraw();
	RedrawManager::currentManager(root)->drawDirtyRegions();
	CPPUNIT_ASSERT_EQUAL((int)7, TestComponent::numDrawCalled);
	RedrawManager::currentManager(root)->drawDirtyRegions();
	CPPUNIT_ASSERT_EQUAL((int)7, TestComponent::numDrawCalled);
	comps[0]->redraw();
	CPPUNIT_ASSERT_EQUAL((int)7, TestComponent::numDrawCalled);
	RedrawManager::currentManager(root)->drawDirtyRegions();
	CPPUNIT_ASSERT_EQUAL((int)8, TestComponent::numDrawCalled);
	comps[2]->redraw();
	RedrawManager::currentManager(root)->drawDirtyRegions();
	CPPUNIT_ASSERT_EQUAL((int)9, TestComponent::numDrawCalled);
	root->validate();
}
//-----------------------------------------------------------------------------
void TestRootPane::testRootPane() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	root->draw( root->getDrawContext() );
	surf->writeToFile(OUTPUT_FOLDER + "/testRootPane.png");
	filesystem::exists(OUTPUT_FOLDER + "/testRootPane.png");
}
//-----------------------------------------------------------------------------
struct TestListener {
	typedef std::stack<
			sambag::disco::components::events::MouseEvent> Events;
	Events lastEvents;
	void onMouseClick(void *,
			const sambag::disco::components::events::MouseEvent &ev)
	{
		lastEvents.push(ev);
	}
};
//-----------------------------------------------------------------------------
void TestRootPane::testMouseEvent() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	using namespace sambag::disco::components::events;
	// init components
	TestListener tl;
	for (int i = 0; i < NUM_COMPOS; ++i) {
		comps[i]->EventSender<MouseEvent>::addEventListener(
				boost::bind(&TestListener::onMouseClick, &tl, _1, _2)
		);
	}
	root->EventSender<MouseEvent>::addEventListener(
			boost::bind(&TestListener::onMouseClick, &tl, _1, _2)
	);
	MouseEventCreator::Ptr evc = MouseEventCreator::create();
    evc->setRootPane(root);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	evc->createPressEvent(50, 50, 1);
	CPPUNIT_ASSERT_EQUAL((size_t)2, tl.lastEvents.size());
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(TestComponent0, Point2D(25, 15), DISCO_MOUSE_PRESSED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(TestComponent0, Point2D(25, 15), DISCO_MOUSE_ENTERED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	evc->createMoveEvent(10, 10);
	CPPUNIT_ASSERT_EQUAL((size_t)3, tl.lastEvents.size());
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(10, 10), DISCO_MOUSE_DRAGGED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(10, 10), DISCO_MOUSE_ENTERED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(TestComponent0, Point2D(-15, -25), DISCO_MOUSE_EXITED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	evc->createReleaseEvent(10, 10, 1);
	CPPUNIT_ASSERT_EQUAL((size_t)1, tl.lastEvents.size());
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(10, 10), DISCO_MOUSE_RELEASED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	evc->createPressEvent(10, 10, 1);
	evc->createReleaseEvent(10, 10, 1);
	CPPUNIT_ASSERT_EQUAL((size_t)3, tl.lastEvents.size());
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(10, 10), DISCO_MOUSE_CLICKED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(10, 10), DISCO_MOUSE_RELEASED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(10, 10), DISCO_MOUSE_PRESSED, 1)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	evc->createMoveEvent(0, 0);
	CPPUNIT_ASSERT_EQUAL((size_t)1, tl.lastEvents.size());
	CPPUNIT_ASSERT_EQUAL(
			std::string("MouseEvent(RootPane, Point2D(0, 0), DISCO_MOUSE_MOVED, 0)"),
			tl.lastEvents.top().toString()
	);
	tl.lastEvents.pop();
	// TODO: test with sub components
	// TODO: test different mouse events
}
//-----------------------------------------------------------------------------
void TestRootPane::testLaf() {
	using namespace sambag::disco::components;
	ui::ALookAndFeel::Ptr laf = LookAndFeelForTest::create();
	ui::UIManager::instance().installLookAndFeel(root, laf);
	CPPUNIT_ASSERT(root->getCurrentLookAndFeel() == laf);
	for (int i = 0; i < NUM_COMPOS; ++i) {
		CPPUNIT_ASSERT(comps[i]->getUI());
	}
	TestComponent::Ptr neu = TestComponent::create();
	CPPUNIT_ASSERT(!neu->getUI());
	root->add(neu);
	CPPUNIT_ASSERT(neu->getUI());
	neu = TestComponent::create();
	TestComponentUI::Ptr cui = TestComponentUI::create();
	neu->setUserUI(cui);
	root->add(neu);
	CPPUNIT_ASSERT(neu->getUI()!=comps[0]->getUI());
}
} //namespace
