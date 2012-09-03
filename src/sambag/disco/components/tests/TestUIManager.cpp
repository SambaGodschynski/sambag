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
	CPPUNIT_ASSERT_EQUAL(x, (int)100);
	CPPUNIT_ASSERT_EQUAL(y, (int)101);
	// reset property
	laf->putProperty("pos.x", 200);
	laf->getProperty("pos.x", x);
	CPPUNIT_ASSERT_EQUAL(x, (int)200);
	// combound type
	Point2D p;
	laf->getProperty("point", p);
	CPPUNIT_ASSERT_EQUAL(p, Point2D(1,2));
}
void TestUIManager::testUIPropertyCache() {
	using namespace sambag::disco;
	using namespace sambag::disco::components::ui;
	UIManager &laf = UIManager::instance();
	laf.putProperty("x", (int)100);
	SAMBAG_PROPERTY_TAG(PropertyTag, "x");
	int x = getUIPropertyCached<PropertyTag>(0);
	CPPUNIT_ASSERT_EQUAL(x, (int)100);
	// change property
	laf.putProperty("x", (int)10);
	x = getUIPropertyCached<PropertyTag>(0);
	// should be the old value until...
	CPPUNIT_ASSERT_EQUAL(x, (int)100);
	// ... reset
	resetUIPorpertyCache();
	x = getUIPropertyCached<PropertyTag>(0);
	CPPUNIT_ASSERT_EQUAL(x, (int)10);
}
} //namespace
