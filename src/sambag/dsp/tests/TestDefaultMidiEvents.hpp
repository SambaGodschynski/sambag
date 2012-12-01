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
	CPPUNIT_TEST_SUITE_END();
public:
	void testEventCopy();
};

} // namespace

#endif /* SAMBAG_TESTDEFAULTMIDIEVENTS_H */
