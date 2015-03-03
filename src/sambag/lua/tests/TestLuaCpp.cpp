/*
 * TestLuaCpp.cpp
 *
 *  Created on: Fri Apr 25 10:05:22 2014
 *      Author: Johannes Unger
 */

#include "TestLuaCpp.hpp"
#include <cppunit/config/SourcePrefix.h>
#include "LuaCppTestClass.hpp"
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestLuaCpp );

namespace tests {

struct TestClass : public LuaCppTestClass {
    typedef boost::shared_ptr<TestClass> Ptr;
    static Ptr create(lua_State *lua) {
        Ptr res(new TestClass());
        res->createLuaObject(lua, "testClass");
        return res;
    }
    int add(lua_State *lua, int a, int b) {
        return a+b;
    }
};

//=============================================================================
//  Class TestLuaCpp
//=============================================================================
//-----------------------------------------------------------------------------
void TestLuaCpp::testLuaCpp() {
    sambag::lua::LuaStateRef lua = sambag::lua::createLuaStateRef();
    TestClass::Ptr tc = TestClass::create(lua.get());
    lua_setglobal(lua.get(), "tObj");
    sambag::lua::executeString(lua.get(), "assert(tObj:add(1,2)==3)");
    sambag::lua::executeString(lua.get(), "assert(tObj.name=='peter')");
}
} //namespace
