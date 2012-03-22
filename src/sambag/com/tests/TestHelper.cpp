/*
 * TestHelper.cpp
 *
 *  Created on: 28.09.2011
 *      Author: samba
 */

#include "TestHelper.hpp"
#include "sambag/com/Helper.hpp"
#include "boost/tuple/tuple.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <list>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestHelper );

using namespace sambag;

namespace tests {
//=============================================================================
void TestHelper::testFromContainer() {
//=============================================================================
	using namespace boost;
	using namespace sambag::com;
	using namespace std;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>prepare data
	typedef list<double> Container;
	Container l;
	l.push_back(0.5); l.push_back(0.6); l.push_back(0.7);
	typedef tuple<double, double, double> Tuple;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert
	double a=0, b=0, c=0;
	tie(a, b, c) = fromContainer<Tuple, Container>(l);
	CPPUNIT_ASSERT_EQUAL( 0.5,  a );
	CPPUNIT_ASSERT_EQUAL( 0.6,  b );
	CPPUNIT_ASSERT_EQUAL( 0.7,  c );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert: N(container) < N(tuple)
	l.remove(0.7);
	a = b = c = 0;
	tie(a, b, c) = fromContainer<Tuple, Container>(l);
	CPPUNIT_ASSERT_EQUAL( 0.5,  a );
	CPPUNIT_ASSERT_EQUAL( 0.6,  b );
	CPPUNIT_ASSERT_EQUAL( 0.0,  c );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert: N(container) > N(tuple)
	a = b = c = 0;
	tie(a, b) = fromContainer<tuple<double, double>, Container>(l);
	CPPUNIT_ASSERT_EQUAL( 0.5,  a );
	CPPUNIT_ASSERT_EQUAL( 0.6,  b );
	CPPUNIT_ASSERT_EQUAL( 0.0,  c );
}
} // namespace tests
