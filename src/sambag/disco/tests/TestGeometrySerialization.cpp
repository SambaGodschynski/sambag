/*
 * TestGeometrySerialization.cpp
 *
 *  Created on: Fri Oct 12 15:20:44 2012
 *      Author: Johannes Unger
 */

#include "TestGeometrySerialization.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/GeometrySerialization.hpp>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestGeometrySerialization );

namespace {
	template <class T>
	void test(const T &obj) {
		std::stringstream ss;
		{
			boost::archive::text_oarchive ar(ss);
			ar<<obj;
		}
		T obj2;
		{
			boost::archive::text_iarchive ar(ss);
			ar>>obj2;
		}
		CPPUNIT_ASSERT_EQUAL(obj, obj2);
	}
}

namespace tests {
//=============================================================================
//  Class TestGeometrySerialization
//=============================================================================
//-----------------------------------------------------------------------------
void TestGeometrySerialization::testSerialization() {
	using namespace sambag::disco;
	Point2D p(10., 15.);
	test(p);
	Dimension d(100., 120.);
	test(d);
	Rectangle r(100., 120., 400., 600.);
	test(r);
	Insets i(15., 10., 100., 100.);
	test(i);
}
} //namespace
