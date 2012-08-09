/*
 * TestWindow.cpp
 *
 *  Created on: Mon Jun 18 15:17:41 2012
 *      Author: Johannes Unger
 */

#include "TestWindow.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/FramedWindow.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/components/Button.hpp>


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestWindow );

namespace tests {
//=============================================================================
//  Class TestWindow
//=============================================================================
//-----------------------------------------------------------------------------
void TestWindow::setUp() {
	using namespace sambag::disco::components;
	if (!wfac)
		wfac = WindowTestToolkit<>::create();
	sambag::disco::components::setWindowToolkit(wfac.get());
}
//-----------------------------------------------------------------------------
void TestWindow::tearDown() {
	sambag::disco::components::setWindowToolkit(NULL);
}
//-----------------------------------------------------------------------------
void TestWindow::testComponentScreenPointConverts() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	Window::Ptr win = FramedWindow::create();
	win->setWindowSize(Dimension(1300,300));
	win->setWindowLocation(Point2D(1000,200));
	win->getLocationOnScreen(Point2D(0,0));
	Button::Ptr btn = Button::create();
	btn->setText("moin");
	win->getContentPane()->add(btn);
	btn = Button::create();
	btn->setText("moin02");
	win->getContentPane()->add(btn);
	win->validate();
	win->open();
	//Window::startMainLoop(); // affects only when create real window
	CPPUNIT_ASSERT_EQUAL(Point2D(1000, 200),
		win->getContentPane()->getLocationOnScreen(Point2D(0,0))
	);
	CPPUNIT_ASSERT_EQUAL(Point2D(0, 0),
		win->getContentPane()->getLocationOnComponent(Point2D(1000,200))
	);
	CPPUNIT_ASSERT_EQUAL(Point2D(1644.5, 205),
		btn->getLocationOnScreen(Point2D(0,0))
	);
	CPPUNIT_ASSERT_EQUAL(Point2D(0, 0),
		btn->getLocationOnComponent(Point2D(1644.5, 205))
	);
}
} //namespace
