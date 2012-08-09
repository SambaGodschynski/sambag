/*
 * TestBoostTimerImpl.hpp
 *
 *  Created on: Thu Aug  9 12:35:18 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTBOOSTTIMERIMPL_H
#define SAMBAG_TESTBOOSTTIMERIMPL_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/Forward.hpp>

namespace tests {
//=============================================================================
class TestBoostTimerImpl : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestBoostTimerImpl );
	CPPUNIT_TEST(testMaxThreads);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testMaxThreads();
};
} // namespace(s)

#endif /* SAMBAG_TESTBOOSTTIMERIMPL_H */
