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
    sambag::lua::LuaStateRef lua;
    static Ptr create() {
        Ptr res(new TestClass());
        res->lua = sambag::lua::createLuaStateRef();
        res->createLuaObject(res->lua.get(), "testClass");
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
}
} //namespace
