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
	SolidPattern::Ptr spattern = SolidPattern::create(ColorRGBA(1));
	CPPUNIT_ASSERT_EQUAL(spattern->getType(), Pattern::SOLID);
	CPPUNIT_ASSERT(spattern->getSolidColor() == ColorRGBA(1));
	// linear
	LinearPattern::Ptr lpattern = LinearPattern::create(Point2D(1,2), Point2D(3,4));
	CPPUNIT_ASSERT_EQUAL(lpattern->getType(), Pattern::LINEAR);
	CPPUNIT_ASSERT(lpattern->getLinearPoints() ==
			LinearPattern::LinearPoints( Point2D(1,2), Point2D(3,4) )
	);
	// radial
	RadialPattern::Ptr rpattern = RadialPattern::create(Point2D(0,1), 1.0, Point2D(2,3), 23.0);
	CPPUNIT_ASSERT_EQUAL(rpattern->getType(), Pattern::RADIAL);
	CPPUNIT_ASSERT(rpattern->getRadialCircles() ==
		RadialPattern::RadialCircles(Point2D(0,1), 1.0, Point2D(2,3), 23.0)
	);
	// surface
	ISurface::Ptr surface = TestSurface::create("testSurface");
	SurfacePattern::Ptr fpattern = SurfacePattern::create(surface);
	CPPUNIT_ASSERT_EQUAL(fpattern->getType(), Pattern::SURFACE);
	CPPUNIT_ASSERT_EQUAL(fpattern->getSurface().get(), surface.get());

}
} // namespace
