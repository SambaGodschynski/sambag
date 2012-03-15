/*
 * TestLuaScript.h
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#ifndef TESTLUASCRIPT_H_
#define TESTLUASCRIPT_H_
#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestLuaScript : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLuaScript );
	CPPUNIT_TEST( testLuaScriptConstruction );
	CPPUNIT_TEST( testRegisterFunction );
	CPPUNIT_TEST( testRegisterFunction02 );
	CPPUNIT_TEST( testRegisterFunction03 );
	CPPUNIT_TEST( testRegisterFunction04 );
	CPPUNIT_TEST( testRegisterFunction05 );
	CPPUNIT_TEST_SUITE_END();
public:
	void testLuaScriptConstruction();
	void testRegisterFunction();
	void testRegisterFunction02();
	void testRegisterFunction03();
	void testRegisterFunction04();
	void testRegisterFunction05();
};
} // namespace
#endif /* TESTLUASCRIPT_H_ */
