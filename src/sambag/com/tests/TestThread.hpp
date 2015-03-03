/*
 * TestThread.hpp
 *
 *  Created on: Fri May 18 15:38:10 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTTHREAD_H
#define SAMBAG_TESTTHREAD_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/com/Thread.hpp>

namespace tests {
//=============================================================================
class TestThread : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestThread );
	CPPUNIT_TEST(testSynchronized);
	CPPUNIT_TEST(testSynchronizedRecursively);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSynchronized();
	void testSynchronizedRecursively();
};

} // namespace

#endif /* SAMBAG_TESTTHREAD_H */
