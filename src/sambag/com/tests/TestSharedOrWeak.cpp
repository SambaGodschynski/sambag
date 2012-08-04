/*
 * TestSharedOrWeak.cpp
 *
 *  Created on: Fri May 18 14:57:39 2012
 *      Author: Johannes Unger
 */

#include "TestSharedOrWeak.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/SharedOrWeak.hpp>
#include <list>
#include <vector>
#include <map>
#include <string>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestSharedOrWeak );

namespace tests {
namespace {
	struct A {
		typedef boost::shared_ptr<A> Ptr;
		typedef boost::weak_ptr<A> WPtr;
		int val;
		A(int v=0) : val(v) {}
	};
	struct B : public A {
		typedef boost::shared_ptr<B> Ptr;
		typedef boost::weak_ptr<B> WPtr;
	};
	int func(const sambag::com::SharedOrWeak<A> &a) {
		return a->val;
	}
}
//=============================================================================
//  Class TestSharedOrWeak
//=============================================================================
//-----------------------------------------------------------------------------
void TestSharedOrWeak::testSharedOrWeak() {
	using namespace sambag::com;
	{
		A::Ptr a(new A());
		a->val = 1;
		SharedOrWeak<A> sw01(a);
		// SharedOrWeak<A> sw02(AsWeakPtr<A>(a)); // TODO: causes weird comp. errors
		//                                        with msvc
		SharedOrWeak<A> sw02 = AsWeakPtr<A>(a);
		CPPUNIT_ASSERT(sw01);
		CPPUNIT_ASSERT(sw02);
		CPPUNIT_ASSERT(sw01 == sw02);
		CPPUNIT_ASSERT(sw01 == a);
		CPPUNIT_ASSERT(a == sw01);
		CPPUNIT_ASSERT_EQUAL((int)1, func(a));
		CPPUNIT_ASSERT_EQUAL((int)1, func(AsWeakPtr<A>(a)));
		CPPUNIT_ASSERT_EQUAL((int)1, func(sw01));
		CPPUNIT_ASSERT_EQUAL((int)1, func(sw02));
		a.reset();
		CPPUNIT_ASSERT(sw01);
		CPPUNIT_ASSERT(sw02);
		sw01.reset();
		CPPUNIT_ASSERT(!sw01);
		CPPUNIT_ASSERT(!sw02);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	{
		B::Ptr b(new B());
		b->val = 1;
		SharedOrWeak<A> sw01(b);                                
		SharedOrWeak<A> sw02 = AsWeakPtr<A>(b);
		CPPUNIT_ASSERT(sw01);
		CPPUNIT_ASSERT(sw02);
		CPPUNIT_ASSERT(sw01 == sw02);
		CPPUNIT_ASSERT_EQUAL((int)1, func(b));
		CPPUNIT_ASSERT_EQUAL((int)1, func(AsWeakPtr<B>(b)));
		CPPUNIT_ASSERT_EQUAL((int)1, func(sw01));
		CPPUNIT_ASSERT_EQUAL((int)1, func(sw02));
		b.reset();
		CPPUNIT_ASSERT(sw01);
		CPPUNIT_ASSERT(sw02);
		sw01.reset();
		CPPUNIT_ASSERT(!sw01);
		CPPUNIT_ASSERT(!sw02);
	}

}
//-----------------------------------------------------------------------------
void TestSharedOrWeak::testSTLContainer() {
	using namespace sambag::com;
	std::vector< SharedOrWeak<A> > v;
	for (int i=0; i<100; ++i) {
		v.push_back( A::Ptr(new A(i)) );
		CPPUNIT_ASSERT_EQUAL(i, v[i]->val);
	}
	A::Ptr a(new A(1));
	v = std::vector< SharedOrWeak<A> >(100, AsWeakPtr<A>(a));
	for (int i=0; i<100; ++i) {
		CPPUNIT_ASSERT(v[i]);
		CPPUNIT_ASSERT_EQUAL((int)1, v[i]->val);
	}
	a.reset();
	for (int i=0; i<100; ++i) {
		CPPUNIT_ASSERT(!v[i]);
	}

	std::map< SharedOrWeak<A>, std::string > m;
	m[a] = "01";
	CPPUNIT_ASSERT_EQUAL((size_t)1, m.size());
	m[AsWeakPtr<A>(a)] = "01";
	CPPUNIT_ASSERT_EQUAL((size_t)1, m.size());
}
} //namespace
