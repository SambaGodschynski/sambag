/*
 * TestGeometrics.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: samba
 */

#include "TestGeometrics.h"

#include "sambag/disco/Geometry.hpp"
using namespace sambag::com;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestGeometrics );

namespace tests {
//=============================================================================
// TestGeometrics::setUp
//=============================================================================
//-----------------------------------------------------------------------------
void TestGeometrics::testRectangle() {
	using namespace sambag::disco;
	Rectangle r;
	Coordinate c = 20.;
	c.type = Coordinate::CM;
	r.setWidth(c);
	Coordinate c2 = r.getWidth();
	CPPUNIT_ASSERT_EQUAL(Coordinate::CM, c2.type);
}
} // namespace
