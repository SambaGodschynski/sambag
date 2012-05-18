/*
 * TestThread.cpp
 *
 *  Created on: Fri May 18 15:38:10 2012
 *      Author: Johannes Unger
 */

#include "TestThread.hpp"
#include <cppunit/config/SourcePrefix.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestThread );

namespace tests {
//=============================================================================
//  Class TestThread
//=============================================================================
//-----------------------------------------------------------------------------
void TestThread::testSynchronized() {
	using namespace sambag::com;
	Mutex mutex;
	int c=0;
	SAMBAG_BEGIN_SYNCHRONIZED(mutex)
		c++;
	SAMBAG_END_SYNCHRONIZED
	CPPUNIT_ASSERT_EQUAL((int)1, c);
	// force deadlock (should throw DeadlockException)
	SAMBAG_BEGIN_SYNCHRONIZED(mutex)
		SAMBAG_BEGIN_SYNCHRONIZED(mutex)
		SAMBAG_END_SYNCHRONIZED
	SAMBAG_END_SYNCHRONIZED
}
} //namespace
