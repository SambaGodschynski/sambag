/*
 * TestPattern.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: samba
 */

#include "TestLuaHelper.hpp"
#include "LuaTestHelper.hpp"
#include "sambag/lua/LuaHelper.hpp"
#include "sambag/lua/LuaSequence.hpp"
#include "sambag/lua/LuaMap.hpp"
#include <boost/foreach.hpp>
#include <string>
#include <sstream>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestLuaHelper );

namespace tests {
//-----------------------------------------------------------------------------
void TestLuaHelper::setUp() {
	L = luaL_newstate();
	luaL_openlibs(L);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::tearDown() {
	lua_close(L);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testLuaMap() {
	using namespace sambag::lua;
	typedef LuaMap<std::string, int> Map;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<insert values
	{ // extra scope
		Map map;
		map["1"] = 10;
		map["2"] = 20;
		map["3"] = 30;
		BOOST_FOREACH(const Map::value_type &it, map) {
			std::stringstream ss;
			ss << it.first;
			int i;
			ss >> i;
			CPPUNIT_ASSERT_EQUAL(i * 10, it.second);
		}
		push(map, L);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<get Values
	Map map;
	CPPUNIT_ASSERT(get(map, L, -1));
	BOOST_FOREACH(const Map::value_type &it, map) {
		std::stringstream ss;
		ss << it.first;
		int i;
		ss >> i;
		CPPUNIT_ASSERT_EQUAL(i * 10, it.second);
	}


}
//-----------------------------------------------------------------------------
void TestLuaHelper::testLuaSequence() {
	using namespace sambag::lua;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<test reference counter
	LuaSequence<int> seq;
	{ // extra scope
		LuaSequence<int> seq2;
		seq2.alloc(10);
		for (int i=0; i<10;++i) seq2[i] = i;
		seq = seq2;
	}
	CPPUNIT_ASSERT_EQUAL((size_t)10, seq.getSize());
	for (int i=0; i<10;++i) {
		CPPUNIT_ASSERT_EQUAL(i, seq[i]); // test read and
		seq[i] = i*2; // write
		CPPUNIT_ASSERT_EQUAL(i*2, seq[i]);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<bounds
	LuaSequence<int> r;
	CPPUNIT_ASSERT_THROW(r[0], OutOfBoundsEx);
	r.alloc(1);
	CPPUNIT_ASSERT_THROW(r[1], OutOfBoundsEx);

}
//-----------------------------------------------------------------------------
void TestLuaHelper::testGet() {
	using namespace sambag::lua;
	lua_pushstring(L, "abc");
	lua_pushnumber(L, 2.5);

	// abc
	int a = 0;
	CPPUNIT_ASSERT(!get(a, L, -2));
	// 2.5
	CPPUNIT_ASSERT(get(a, L, -1));
	CPPUNIT_ASSERT_EQUAL((int)2, a);
	double b = 0;
	CPPUNIT_ASSERT(get(b, L, -1));
	CPPUNIT_ASSERT_EQUAL((double)2.5, b);
	std::string c;
	CPPUNIT_ASSERT(get<std::string>(c, L, -1));
	CPPUNIT_ASSERT_EQUAL(std::string("2.5"), c);
	LuaSequence<float> d;
	CPPUNIT_ASSERT(!get(d, L, -2));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<checktable
	{ // extra scope
		LuaSequence<int> arr(10);
		for (size_t i=0; i<arr.getSize(); ++i) arr[i] = i*10;
		push(arr,L);
	}
	LuaSequence<int> arr;
	CPPUNIT_ASSERT(check<ILuaTable>(L, -1));
	CPPUNIT_ASSERT(get(arr, L, -1));
	for (size_t i=0; i<arr.getSize(); ++i) {
		CPPUNIT_ASSERT_EQUAL((int)i*10, arr[i]);
	}
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testMultiPushGet() {
	using namespace sambag::lua;
	typedef LuaMap<std::string, int> Map;
	{ // extra scope
		int a = 1;
		double b = 2.0;
		std::string c = "abc";
		LuaSequence<int> d(10);
		for (size_t i=0; i<d.getSize(); ++i) d[i] = i*10;
		Map e;
		e["1"] = 10;
		e["2"] = 20;
		e["3"] = 30;
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<push content
		push(a,b,c,d,e,L);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<get content
	int a; double b; std::string c; LuaSequence<int> d; Map e;
	CPPUNIT_ASSERT(get(e,d,c,b,a,L,-1));
	CPPUNIT_ASSERT_EQUAL((int)1, a);
	CPPUNIT_ASSERT_EQUAL((double)2.0, b);
	CPPUNIT_ASSERT_EQUAL(std::string("abc"), c);
	for (size_t i=0; i<d.getSize(); ++i) {
		CPPUNIT_ASSERT_EQUAL((int)i*10, d[i]);
	}
	BOOST_FOREACH(const Map::value_type &it, e) {
		std::stringstream ss;
		ss << it.first;
		int i;
		ss >> i;
		CPPUNIT_ASSERT_EQUAL(i * 10, it.second);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<test failure
	CPPUNIT_ASSERT(!get(a,d,c,b,a,L,-1)); // 1st wrong
	CPPUNIT_ASSERT(!get(e,a,c,b,a,L,-1)); // 2nd wrong
	CPPUNIT_ASSERT(!get(a,d,e,b,a,L,-1)); // 3th wrong
	CPPUNIT_ASSERT(!get(a,d,c,e,a,L,-1)); // 4th wrong
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-1)); // 5th wrong
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-2)); // wrong index
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-3)); // wrong index
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-4)); // wrong index
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-5)); // wrong index
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCheckType() {
	using namespace sambag::lua;
	lua_pushstring(L, "abc");
	lua_pushnumber(L, 0.5);
	// abc
	CPPUNIT_ASSERT(!check<int>(L, -2));
	CPPUNIT_ASSERT(check<std::string>(L, -2));
	// 0.5
	CPPUNIT_ASSERT(check<int>(L, -1));
	CPPUNIT_ASSERT(check<double>(L, -1));
	CPPUNIT_ASSERT(check<std::string>(L, -1)); // number is string convertible
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<checktable
	LuaSequence<int> seq(10);
	for (int i=0; i<10;++i) seq[i] = i;
	push(seq,L);
	CPPUNIT_ASSERT(check<ILuaTable>(L, -1));
	CPPUNIT_ASSERT(!check<ILuaTable>(L, -2));
}
} // namespace
