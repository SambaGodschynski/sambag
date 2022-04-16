/*
 * TestBoostTimer2.hpp
 *
 *  Created on: Sun Jul 14 17:01:01 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTBOOSTTIMER2_H
#define SAMBAG_TESTBOOSTTIMER2_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/com/UnitEx.hpp>

namespace tests {
//=============================================================================
class TestBoostTimer2 : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestBoostTimer2 );
    CPPUNIT_TEST( testStartTimer );
    CPPUNIT_TEST_KNOWN_ISSUE( testStopTimer );
    CPPUNIT_TEST_KNOWN_ISSUE( testRestartTimer );
    CPPUNIT_TEST( testFailure );
	CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
	void testStartTimer();
    void testStopTimer();
    void testRestartTimer();
    void testFailure();
};

} // namespace

#endif /* SAMBAG_TESTBOOSTTIMER2_H */
