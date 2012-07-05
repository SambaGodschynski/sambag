/*
 * TestMouseEvent.hpp
 *
 *  Created on: Thu Jul  5 11:41:56 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTMOUSEEVENT_H
#define SAMBAG_TESTMOUSEEVENT_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestMouseEvent : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestMouseEvent );
	CPPUNIT_TEST( testEventSwitch );
	CPPUNIT_TEST_SUITE_END();
public:
	void testEventSwitch();
};

} // namespace

#endif /* SAMBAG_TESTMOUSEEVENT_H */
