/*
 * TestLuaHelper.h
 *
 *  Created on: Feb 13, 2012
 *      Author: samba
 */

#ifndef TESTLUAHELPER_H_
#define TESTLUAHELPER_H_

#include <cppunit/extensions/HelperMacros.h>

struct lua_State;

namespace tests {
//=============================================================================
class TestLuaHelper : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLuaHelper );
	CPPUNIT_TEST( testLuaSequenceEx );
	CPPUNIT_TEST( testLuaMap );
	CPPUNIT_TEST( testGet );
	CPPUNIT_TEST( testCheckType );
	CPPUNIT_TEST( testMultiPushGet );
	CPPUNIT_TEST( testCallF );
	CPPUNIT_TEST_SUITE_END();
	lua_State *L;
public:
	virtual void setUp();
	virtual void tearDown();
	void testLuaSequenceEx();
	void testLuaMap();
	void testGet();
	void testMultiPushGet();
	void testCheckType();
	void testCallF();
};

} // namespace
#endif /* TESTLUAHELPER_H_ */
