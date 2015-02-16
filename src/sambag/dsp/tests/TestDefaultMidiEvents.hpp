/*
 * TestDefaultMidiEvents.hpp
 *
 *  Created on: Fri Nov 30 14:19:00 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTDEFAULTMIDIEVENTS_H
#define SAMBAG_TESTDEFAULTMIDIEVENTS_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestDefaultMidiEvents : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestDefaultMidiEvents );
	CPPUNIT_TEST( testEventCopy );
	CPPUNIT_TEST( testEventCopyFiltered );
	CPPUNIT_TEST( testEventCopyFiltered02 );
	CPPUNIT_TEST( testEventCopyFiltered03 );
	CPPUNIT_TEST( testEventCopyFiltered04 );
	CPPUNIT_TEST( testEventCopyFiltered05 );
	CPPUNIT_TEST( testEventCopyFiltered06 );
    CPPUNIT_TEST( testCreateFlatRawData );
    CPPUNIT_TEST( testInsertDeep );
    CPPUNIT_TEST( testHelper );
	CPPUNIT_TEST_SUITE_END();
public:
	void testEventCopy();
	void testEventCopyFiltered();
	void testEventCopyFiltered02();
	void testEventCopyFiltered03();
	void testEventCopyFiltered04();
	void testEventCopyFiltered05();
	void testEventCopyFiltered06();
    void testCreateFlatRawData();
    void testInsertDeep();
    void testHelper();
};

} // namespace

#endif /* SAMBAG_TESTDEFAULTMIDIEVENTS_H */
