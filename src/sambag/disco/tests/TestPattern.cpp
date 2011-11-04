/*
 * TestPattern.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: samba
 */

#include "TestPattern.h"
#include "sambag/com/Common.hpp"
#include <boost/tuple/tuple_comparison.hpp>
#include "HelperForTesting.hpp"

using namespace sambag::com;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestPattern );

namespace tests {
//-----------------------------------------------------------------------------
void TestPattern::testConstruction() {
	using namespace sambag::disco;
	using namespace boost::tuples;
	// solid
	Pattern::Ptr pattern = Pattern::createSolid(ColorRGBA(1));
	CPPUNIT_ASSERT_EQUAL(pattern->getType(), Pattern::SOLID);
	CPPUNIT_ASSERT(pattern->getSolidColor() == ColorRGBA(1));
	// linear
	pattern = Pattern::createLinear(Point2D(1,2), Point2D(3,4));
	CPPUNIT_ASSERT_EQUAL(pattern->getType(), Pattern::LINEAR);
	CPPUNIT_ASSERT(pattern->getLinearPoints() ==
		Pattern::LinearPoints( Point2D(1,2), Point2D(3,4) )
	);
	// radial
	pattern = Pattern::createRadial(Point2D(0,1), 1.0, Point2D(2,3), 23.0);
	CPPUNIT_ASSERT_EQUAL(pattern->getType(), Pattern::RADIAL);
	CPPUNIT_ASSERT(pattern->getRadialCircles() ==
		Pattern::RadialCircles(Point2D(0,1), 1.0, Point2D(2,3), 23.0)
	);
	// surface
	ISurface::Ptr surface = TestSurface::create("testSurface");
	pattern = Pattern::createSurface(surface);
	CPPUNIT_ASSERT_EQUAL(pattern->getType(), Pattern::SURFACE);
	CPPUNIT_ASSERT_EQUAL(pattern->getSurface().get(), surface.get());
	// force error
	CPPUNIT_ASSERT_THROW(pattern->getRadialCircles(), Pattern::TypeMismatch);

}
} // namespace
