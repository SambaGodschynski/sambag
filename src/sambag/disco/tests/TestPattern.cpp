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
	ISolidPattern::Ptr spattern = fac->createSolidPattern(ColorRGBA(1));
	CPPUNIT_ASSERT(spattern->getSolidColor() == ColorRGBA(1));
	// linear
	ILinearPattern::Ptr lpattern =
		fac->createLinearPattern(Point2D(1,2), Point2D(3,4));
	CPPUNIT_ASSERT(lpattern->getLinearPoints() ==
			ILinearPattern::LinearPoints( Point2D(1,2), Point2D(3,4) )
	);
	// radial
	IRadialPattern::Ptr rpattern =
		fac->createRadialPattern(Point2D(0,1), 1.0, Point2D(2,3), 23.0);
	CPPUNIT_ASSERT(rpattern->getRadialCircles() ==
		IRadialPattern::RadialCircles(Point2D(0,1), 1.0, Point2D(2,3), 23.0)
	);
	// surface
	ISurface::Ptr surface = fac->createImageSurface(300, 200);
	ISurfacePattern::Ptr fpattern = fac->createSurfacePattern(surface);
	CPPUNIT_ASSERT(fpattern->getSurface());

}
} // namespace
