/*
 * TestLookAndFeel.cpp
 *
 *  Created on: Sun Jun  3 15:28:03 2012
 *      Author: Johannes Unger
 */

#include "TestLookAndFeel.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/ui/ALookAndFeel.hpp>
#include "TestComponents.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestLookAndFeel );


namespace tests {
//=============================================================================
//  Class TestLookAndFeel
//=============================================================================
//-----------------------------------------------------------------------------
void TestLookAndFeel::testGetUI() {
	LookAndFeelForTest::Ptr laf = LookAndFeelForTest::create();
	CPPUNIT_ASSERT(laf->getUI<TestComponent>());
	CPPUNIT_ASSERT(!laf->getUI<sambag::disco::components::AComponent>());
}
} //namespace
