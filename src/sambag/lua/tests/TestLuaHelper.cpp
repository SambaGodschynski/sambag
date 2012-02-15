/*
 * TestPattern.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: samba
 */

#include "TestLuaHelper.hpp"
#include "LuaTestHelper.hpp"
#include "sambag/lua/LuaHelper.hpp"
#include <string>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestLuaHelper );

namespace tests {
//-----------------------------------------------------------------------------
void TestLuaHelper::testGet() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
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
	LuaTableArray<float> d;
	CPPUNIT_ASSERT(!get(d, L, -1));
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCheckType() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
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
	lua_close(L);
}
} // namespace
