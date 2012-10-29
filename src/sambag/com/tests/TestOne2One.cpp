/*
 * TestOne2One.cpp
 *
 *  Created on: Thu Oct 18 14:27:42 2012
 *      Author: Johannes Unger
 */

#include "TestOne2One.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/One2One.hpp>
#include <boost/shared_ptr.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestOne2One );


struct CustomKey {
	int value;
	CustomKey(int k=0) : value(k) {}
	bool operator==(const CustomKey &k) const {
		return value == k.value;
	}
};

size_t hash_value(const CustomKey &key) {
	return key.value;
}

namespace tests {
//=============================================================================
//  Class TestOne2One
//=============================================================================
namespace {
	struct Test {
		typedef boost::shared_ptr<Test> Ptr;
		static Ptr create() {
			return Ptr(new Test());
		}
	};
	template <class T>
	struct TestCreator {
		Test::Ptr create(const T &key) {
			return Test::create();
		}
	};
}
//-----------------------------------------------------------------------------
void TestOne2One::testSimple() {
	using namespace sambag::com;
	One2One<Test::Ptr, TestCreator, int> o2o;
	Test::Ptr objectA = o2o.get(0);
	Test::Ptr objectB = o2o.get(1);
	Test::Ptr objectC = o2o.get(1);
	SAMBAG_ASSERT(objectA != objectB);
	SAMBAG_ASSERT(objectB == objectC);
}
//-----------------------------------------------------------------------------
void TestOne2One::testCustom() {
	using namespace sambag::com;
	One2One<Test::Ptr, TestCreator, CustomKey> o2o;
	Test::Ptr objectA = o2o.get(CustomKey(0));
	Test::Ptr objectB = o2o.get(CustomKey(1));
	Test::Ptr objectC = o2o.get(CustomKey(1));
	SAMBAG_ASSERT(objectA != objectB);
	SAMBAG_ASSERT(objectB == objectC);
}
} //namespace