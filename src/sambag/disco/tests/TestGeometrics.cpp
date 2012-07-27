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
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<dimension
	Rectangle r;
	r = Rectangle(Point2D(0.,0.), Point2D(10., 10.));
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.getDimension());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<xchange min and max corner.
	r = Rectangle(Point2D(10.,10.), Point2D(0., 0.));
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.getDimension());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<xchange min and max corner.
	r = Rectangle(Point2D(10.,10.), Point2D(0., 0.));
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.getDimension());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<check handling of negative w/h
	r = Rectangle(Point2D(0.,10.), Point2D(10., 0.));
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.getDimension());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<check handling of negative w/h constructor: p,w,h
	r = Rectangle(Point2D(10.,10.), -10., -10.);
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.getDimension());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<check handling of negative w/h constructor: x,y,w,h
	r = Rectangle(10.,10., -10., -10.);
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.getDimension());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
}
} // namespace
