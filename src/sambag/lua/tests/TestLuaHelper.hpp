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
	CPPUNIT_TEST( testNestedSequences );
	CPPUNIT_TEST( testNestedSequencesEx );
	CPPUNIT_TEST( testGetGlobal );
	CPPUNIT_TEST( testCallF01 );
	CPPUNIT_TEST( testCallF02 );
	CPPUNIT_TEST( testCallF03 );
	CPPUNIT_TEST( testCallF04 );
	CPPUNIT_TEST( testCallF05 );
	CPPUNIT_TEST_SUITE_END();
	lua_State *L;
public:
	virtual void setUp();
	virtual void tearDown();
	void testLuaStateRef();
	void testLuaSequenceEx();
	void testLuaMap();
	void testNestedSequences();
	void testNestedSequencesEx();
	void testGet();
	void testMultiPushGet();
	void testCheckType();
	void testGetGlobal();
	void testCallF();
	void testCallF01();
	void testCallF02();
	void testCallF03();
	void testCallF04();
	void testCallF05();
};

} // namespace
#endif /* TESTLUAHELPER_H_ */
