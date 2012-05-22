/*
 * TestUIManager.cpp
 *
 *  Created on: Sun Jun  3 17:19:41 2012
 *      Author: Johannes Unger
 */

#include "TestUIManager.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/ui/UIManager.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestUIManager );

namespace tests {
//=============================================================================
//  Class TestUIManager
//=============================================================================
//-----------------------------------------------------------------------------
void TestUIManager::testSingleton() {
	using namespace sambag::disco::components::ui;
	CPPUNIT_ASSERT( &(UIManager::instance()) != NULL);
}
} //namespace
