/*
 * TestLuaScript.h
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#ifndef TESTLUASCRIPT_H_
#define TESTLUASCRIPT_H_
#include <cppunit/extensions/HelperMacros.h>

struct lua_State;

namespace tests {
//=============================================================================
class TestLuaScript : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLuaScript );
	CPPUNIT_TEST( testLuaScript );

	CPPUNIT_TEST_SUITE_END();
	lua_State *L;
public:
	void testLuaScript();
};

#endif /* TESTLUASCRIPT_H_ */
