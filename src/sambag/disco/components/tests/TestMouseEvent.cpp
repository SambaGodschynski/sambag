/*
 * TestMouseEvent.cpp
 *
 *  Created on: Thu Jul  5 11:41:56 2012
 *      Author: Johannes Unger
 */

#include "TestMouseEvent.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/events/MouseEvent.hpp>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestMouseEvent );

namespace dev = sambag::disco::components::events;

namespace tests {
struct Listener {
	int called;
	Listener() : called(0) {}
	void mousePressed(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_PRESSED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_PRESSED);
	}
	void mouseReleased(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_RELEASED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_RELEASED);
	}
	void mouseEntered(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_ENTERED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_ENTERED);
	}
	void mouseExited(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_EXITED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_EXITED);
	}
	void mouseClicked(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_CLICKED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_CLICKED);
	}
	void mouseMoved(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_MOVED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_MOVED);
	}
	void mouseDragged(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_DRAGGED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_DRAGGED);
	}
	void mouseWheelMoved(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::MOUSE_WHEEL;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::MOUSE_WHEEL);
	}
};
//=============================================================================
//  Class TestMouseEvent
//=============================================================================
//-----------------------------------------------------------------------------
void TestMouseEvent::testEventSwitch() {
	using namespace sambag::disco;
	using namespace sambag::disco::components::events;
	{
		Listener l;
		MouseEvent ev(Point2D(), MouseEvent::BUTTON1, MouseEvent::MOUSE_PRESSED);
		MouseEventSwitch<>::delegate(ev, l);
		MouseEventSwitch<MouseEvent::MOUSE_RELEASED>::delegate(ev, l);
		CPPUNIT_ASSERT_EQUAL(l.called, (int)MouseEvent::MOUSE_PRESSED);
	}
}
} //namespace
