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
#include <boost/assign/std/vector.hpp>
#include <list>
#include <vector>
#include <sstream>
#include <string>

using namespace sambag;

namespace tests {
namespace {
struct ToString {
	std::stringstream ss;
	int index;
	template<typename T>
	void operator()(const T &value) {
		ss << index++ << ": " << value << "; ";
	}
	ToString() :
		index(0) {
	}
};
}
//=============================================================================
void TestHelper::testTupleForeach() {
//=============================================================================
	using namespace boost;
	using namespace sambag::com;
	using namespace std;
	ToString tStr;
	tuple<int, float, string> tpl(10, 1.1f, "inhaltslosabernichtleer");
	foreach(tpl, tStr);
	CPPUNIT_ASSERT_EQUAL(
			string("0: 10; 1: 1.1; 2: inhaltslosabernichtleer; "),
			tStr.ss.str()
	);
}
//=============================================================================
void TestHelper::testFromContainer() {
	//=============================================================================
	using namespace boost;
	using namespace sambag::com;
	using namespace std;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>prepare data
	typedef list<double> Container;
	Container l;
	l.push_back(0.5);
	l.push_back(0.6);
	l.push_back(0.7);
	typedef tuple<double, double, double> Tuple;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert
	double a = 0, b = 0, c = 0;
	tie(a, b, c) = fromContainer<Tuple, Container> (l);
	CPPUNIT_ASSERT_EQUAL( 0.5, a );
	CPPUNIT_ASSERT_EQUAL( 0.6, b );
	CPPUNIT_ASSERT_EQUAL( 0.7, c );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert: N(container) < N(tuple)
	l.remove(0.7);
	a = b = c = 0;
	tie(a, b, c) = fromContainer<Tuple, Container> (l);
	CPPUNIT_ASSERT_EQUAL( 0.5, a );
	CPPUNIT_ASSERT_EQUAL( 0.6, b );
	CPPUNIT_ASSERT_EQUAL( 0.0, c );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert: N(container) > N(tuple)
	a = b = c = 0;
	tie(a, b) = fromContainer<tuple<double, double> , Container> (l);
	CPPUNIT_ASSERT_EQUAL( 0.5, a );
	CPPUNIT_ASSERT_EQUAL( 0.6, b );
	CPPUNIT_ASSERT_EQUAL( 0.0, c );
}
//-----------------------------------------------------------------------------
void TestHelper::testIndexOf() {
	using namespace boost::assign;
	using namespace sambag::com;
	{
		std::vector<int> v;
		v += 1, 2, 3, 4, 5, 6, 7, 8, 9;
		CPPUNIT_ASSERT_EQUAL((int)0, indexOf(v, 1));
		CPPUNIT_ASSERT_EQUAL((int)8, indexOf(v, 9));
		CPPUNIT_ASSERT_EQUAL((int)4, indexOf(v, 5));
		CPPUNIT_ASSERT_EQUAL((int)-1, indexOf(v, 10));
	}
	{
		std::vector<std::string> v;
		v += "a", "b", "c";
		CPPUNIT_ASSERT_EQUAL((int)0, indexOf(v, "a"));
		CPPUNIT_ASSERT_EQUAL((int)1, indexOf(v, "b"));
		CPPUNIT_ASSERT_EQUAL((int)2, indexOf(v, "c"));
		CPPUNIT_ASSERT_EQUAL((int)-1, indexOf(v, "d"));
	}
}
} // namespace tests
