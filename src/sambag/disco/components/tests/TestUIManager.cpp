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
//-----------------------------------------------------------------------------
void TestUIManager::testSetGetProperty() {
	using namespace sambag::disco;
	using namespace sambag::disco::components::ui;
	UIManager *laf = &(UIManager::instance());
	laf->putProperty("pos.x", 100);
	laf->putProperty("pos.y", 101);
	laf->putProperty("point", Point2D(1,2));
	int x=0, y =0;
	laf->getProperty("pos.x", x);
	laf->getProperty("pos.y", y);
	CPPUNIT_ASSERT_EQUAL((int)100, x);
	CPPUNIT_ASSERT_EQUAL((int)101, y);
	// reset property
	laf->putProperty("pos.x", 200);
	laf->getProperty("pos.x", x);
	CPPUNIT_ASSERT_EQUAL((int)200, x);
	// combound type
	Point2D p;
	laf->getProperty("point", p);
	CPPUNIT_ASSERT_EQUAL(Point2D(1,2), p);
}
namespace {
	SAMBAG_PROPERTY_TAG(PropertyTag, "x");
} // namespace

void TestUIManager::testUIPropertyCache() {
	using namespace sambag::disco;
	using namespace sambag::disco::components::ui;
	UIManager &laf = UIManager::instance();
	laf.putProperty("x", (int)100);
	int x = getUIPropertyCached<PropertyTag>(0);
	CPPUNIT_ASSERT_EQUAL((int)100, x);
	// change property
	laf.putProperty("x", (int)10);
	x = getUIPropertyCached<PropertyTag>(0);
	// should be the old value until...
	CPPUNIT_ASSERT_EQUAL((int)100, x);
	// ... reset
	resetUIPorpertyCache();
	x = getUIPropertyCached<PropertyTag>(0);
	CPPUNIT_ASSERT_EQUAL((int)10, x);
}
} //namespace
