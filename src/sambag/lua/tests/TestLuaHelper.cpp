/*
 * TestPattern.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: samba
 */

#include "TestLuaHelper.hpp"
#include "LuaTestHelper.hpp"
#include "sambag/lua/LuaHelper.hpp"
#include "sambag/lua/LuaTable.hpp"
#include <string>

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
void TestLuaHelper::testGet02() {
	using namespace sambag::lua;
	lua_pushstring(L, "abc");
	lua_pushnumber(L, 10);
	lua_pushnumber(L, 2);
	lua_pushnumber(L, 1);
	std::string a;
	int b = 0, c = 0, d = 0;
	CPPUNIT_ASSERT_EQUAL((int)0, get(d,c,b,a,L,-1));
	CPPUNIT_ASSERT_EQUAL((int)1, d);
	CPPUNIT_ASSERT_EQUAL((int)2, c);
	CPPUNIT_ASSERT_EQUAL((int)10, b);
	CPPUNIT_ASSERT_EQUAL(std::string("abc"), a);
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
	lua_newtable(L);
	int top = lua_gettop(L);
	for (int i=0; i<10; ++i) {
		lua_pushnumber(L, i);
	    lua_pushnumber(L, 1./((float)i+1));
	    lua_settable(L, top);
	}
	CPPUNIT_ASSERT(check<ILuaTable>(L, -1));
	CPPUNIT_ASSERT(!check<ILuaTable>(L, -2));
}
} // namespace
