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
#include "sambag/disco/IDiscoFactory.hpp"
using namespace sambag::com;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestPattern );

namespace tests {
//-----------------------------------------------------------------------------
void TestPattern::testConstruction() {
	using namespace sambag::disco;
	using namespace boost::tuples;
	// solid
	IDiscoFactory::Ptr fac = getDiscoFactory();
	ASolidPattern::Ptr spattern = fac->createSolidPattern(ColorRGBA(1));
	CPPUNIT_ASSERT_EQUAL(spattern->getType(), IPattern::SOLID);
	CPPUNIT_ASSERT(spattern->getSolidColor() == ColorRGBA(1));
	// linear
	ALinearPattern::Ptr lpattern =
		fac->createLinearPattern(Point2D(1,2), Point2D(3,4));
	CPPUNIT_ASSERT_EQUAL(lpattern->getType(), IPattern::LINEAR);
	CPPUNIT_ASSERT(lpattern->getLinearPoints() ==
			ALinearPattern::LinearPoints( Point2D(1,2), Point2D(3,4) )
	);
	// radial
	ARadialPattern::Ptr rpattern =
		fac->createRadialPattern(Point2D(0,1), 1.0, Point2D(2,3), 23.0);
	CPPUNIT_ASSERT_EQUAL(rpattern->getType(), IPattern::RADIAL);
	CPPUNIT_ASSERT(rpattern->getRadialCircles() ==
		ARadialPattern::RadialCircles(Point2D(0,1), 1.0, Point2D(2,3), 23.0)
	);
	// surface
	ISurface::Ptr surface = fac->createImageSurface(300, 200);
	ASurfacePattern::Ptr fpattern = fac->createSurfacePattern(surface);
	CPPUNIT_ASSERT_EQUAL(fpattern->getType(), IPattern::SURFACE);
	CPPUNIT_ASSERT(fpattern->getSurface());

}
} // namespace
