/*
 * TestPopupMenu.cpp
 *
 *  Created on: Mon Jun 18 12:11:30 2012
 *      Author: Johannes Unger
 */

#include "TestPopupMenu.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/PopupMenu.hpp>


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestPopupMenu );

namespace tests {
//=============================================================================
//  Class TestPopupMenu
//=============================================================================
//-----------------------------------------------------------------------------
void TestPopupMenu::testAddElements() {
	using namespace sambag::disco::components;
	PopupMenu::Ptr m = PopupMenu::create();
	static size_t NUM_EL = 10;
	for (size_t i=0; i<NUM_EL; ++i) {
		MenuItem::Ptr item = MenuItem::create();
		m->add(item);
	}
	AComponent::Ptr item = MenuItem::create();
	m->AContainer::add(item);
	IMenuElement::MenuElements l;
	m->getSubElements(l);
	CPPUNIT_ASSERT_EQUAL(NUM_EL + 1, l.size());
}
} //namespace
