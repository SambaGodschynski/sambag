/*
 * TestGeometrics.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: samba
 */

#include "TestGeometrics.hpp"

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
    //<<<<<<<<<<<<<<<<<<<<<<<<<check nesting
    r = Rectangle(0.,0.,20.,20.);
    CPPUNIT_ASSERT_EQUAL(Rectangle(10,15,20,20), r.x(10).y(15));
    CPPUNIT_ASSERT_EQUAL(Rectangle(10,15,30,35), r.width(30).height(35));
}
//-----------------------------------------------------------------------------
void TestGeometrics::testXimize() {
	using namespace sambag::disco;
	CPPUNIT_ASSERT_EQUAL(Point2D(), minimize(Point2D(), Point2D()));
	CPPUNIT_ASSERT_EQUAL(Point2D(), maximize(Point2D(), Point2D()));
	CPPUNIT_ASSERT_EQUAL(Point2D(10,15), minimize(Point2D(10,15), Point2D(11,16)));
	CPPUNIT_ASSERT_EQUAL(Point2D(11,16), maximize(Point2D(10,15), Point2D(11,16)));
	CPPUNIT_ASSERT_EQUAL(Point2D(10,14), minimize(Point2D(10,15), Point2D(11,14)));
	CPPUNIT_ASSERT_EQUAL(Point2D(11,15), maximize(Point2D(10,15), Point2D(11,14)));
	CPPUNIT_ASSERT_EQUAL(Point2D(10,14), minimize(Point2D(11,14), Point2D(10,15)));
	CPPUNIT_ASSERT_EQUAL(Point2D(11,16), maximize(Point2D(10,16), Point2D(11,14)));
	CPPUNIT_ASSERT_EQUAL(Point2D(11,14), minimize(Point2D(9999999,9999999), Point2D(11,14)));
	CPPUNIT_ASSERT_EQUAL(Point2D(11,14), maximize(Point2D(-9999999,-9999999), Point2D(11,14)));
}
} // namespace
