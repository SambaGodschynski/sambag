/*
 * TestLuaRegisterFunction.h
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#ifndef TESTLUAREGISTERFUNCTION_H_
#define TESTLUAREGISTERFUNCTION_H_
#include <cppunit/extensions/HelperMacros.h>
#include <lua.hpp>

namespace tests {
//=============================================================================
class TestLuaScript : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLuaScript );
	CPPUNIT_TEST( testRegisterFunction );
	CPPUNIT_TEST( testRegisterFunction02 );
	CPPUNIT_TEST( testRegisterFunction03 );
	CPPUNIT_TEST( testRegisterFunction04 );
	CPPUNIT_TEST( testRegisterFunction05 );
	CPPUNIT_TEST( testReturnTuple01 );
	CPPUNIT_TEST_SUITE_END();
	lua_State *L;
public:
	virtual void setUp();
	virtual void tearDown();
	void testRegisterFunction();
	void testRegisterFunction02();
	void testRegisterFunction03();
	void testRegisterFunction04();
	void testRegisterFunction05();
	void testReturnTuple01();
};
} // namespace
#endif /* TESTLUAREGISTERFUNCTION_H_ */
