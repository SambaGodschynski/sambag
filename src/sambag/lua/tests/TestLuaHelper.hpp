/*
 * TestLuaHelper.h
 *
 *  Created on: Feb 13, 2012
 *      Author: samba
 */

#ifndef TESTLUAHELPER_H_
#define TESTLUAHELPER_H_

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestLuaHelper : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestLuaHelper );
	CPPUNIT_TEST( testGet );
	CPPUNIT_TEST( testCheckType );
	CPPUNIT_TEST_SUITE_END();
public:
	void testGet();
	void testCheckType();
};

} // namespace
#endif /* TESTLUAHELPER_H_ */
