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
		called |= dev::MouseEvent::DISCO_MOUSE_PRESSED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_PRESSED);
	}
	void mouseReleased(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_RELEASED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_RELEASED);
	}
	void mouseEntered(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_ENTERED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_ENTERED);
	}
	void mouseExited(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_EXITED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_EXITED);
	}
	void mouseClicked(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_CLICKED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_CLICKED);
	}
	void mouseMoved(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_MOVED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_MOVED);
	}
	void mouseDragged(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_DRAGGED;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_DRAGGED);
	}
	void mouseWheelMoved(const dev::MouseEvent &ev) {
		called |= dev::MouseEvent::DISCO_MOUSE_WHEEL;
		CPPUNIT_ASSERT_EQUAL(ev.getType(), dev::MouseEvent::DISCO_MOUSE_WHEEL);
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
		MouseEvent ev(Point2D(), MouseEvent::DISCO_BTN1, MouseEvent::DISCO_MOUSE_PRESSED);
		MouseEventSwitch<>::delegate(ev, l);
		MouseEventSwitch<MouseEvent::DISCO_MOUSE_RELEASED>::delegate(ev, l);
		CPPUNIT_ASSERT_EQUAL(l.called, (int)MouseEvent::DISCO_MOUSE_PRESSED);
	}
}
} //namespace
