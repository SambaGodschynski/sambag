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
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.size());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<xchange min and max corner.
	r = Rectangle(Point2D(10.,10.), Point2D(0., 0.));
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.size());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<check handling of negative w/h
	r = Rectangle(Point2D(0.,10.), Point2D(10., 0.));
	CPPUNIT_ASSERT(Dimension(10., 10.) == r.size());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x0());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<check handling of negative w/h constructor: p,w,h
	r = Rectangle(Point2D(10.,10.), -10., -10.);
	CPPUNIT_ASSERT(Dimension(-10., -10.) == r.size());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x0());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<check handling of negative w/h constructor: x,y,w,h
	r = Rectangle(10.,10., -10., -10.);
	CPPUNIT_ASSERT(Dimension(-10., -10.) == r.size());
	CPPUNIT_ASSERT(Point2D(10., 10.)==r.x0());
	CPPUNIT_ASSERT(Point2D(0., 0.)==r.x1());
	//<<<<<<<<<<<<<<<<<<<<<<<<<check insets
	r = Rectangle(0.,0.,10.,10.);
	r.inset(5., 5.);
	CPPUNIT_ASSERT_EQUAL(Dimension(15., 15.), r.size());
	CPPUNIT_ASSERT_EQUAL(Point2D(-2.5, -2.5), r.x0());
	CPPUNIT_ASSERT_EQUAL(Point2D(12.5, 12.5), r.x1());
}
} // namespace
