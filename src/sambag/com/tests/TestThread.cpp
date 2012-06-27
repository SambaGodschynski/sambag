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

void callLockRecursive(sambag::com::RecursiveMutex &mutex, int i=0) {
	if (i==10)
		return;
	using namespace sambag::com;
	SAMBAG_BEGIN_SYNCHRONIZED(mutex)
		callLockRecursive(mutex, i+1);
	SAMBAG_END_SYNCHRONIZED
}

struct Deadlock {
	sambag::com::RecursiveMutex &mutex;
	bool &deadlockExThrown;
	void operator()() {
		try {
			SAMBAG_BEGIN_SYNCHRONIZED(mutex)
					// value to low -> no deadlock
					sambag::com::wait(5);
			SAMBAG_END_SYNCHRONIZED
		} catch (const sambag::com::DeadLockException &ex) {
			deadlockExThrown = true;
		}
	}
	Deadlock(sambag::com::RecursiveMutex &mutex, bool &deadlockExThrown) :
		mutex(mutex), deadlockExThrown(deadlockExThrown) {}
};

namespace tests {
//=============================================================================
//  Class TestThread
//=============================================================================
//-----------------------------------------------------------------------------
void TestThread::testSynchronized() {
	using namespace sambag::com;
	RecursiveMutex mutex;
	int c=0;
	SAMBAG_BEGIN_SYNCHRONIZED(mutex)
		c++;
	SAMBAG_END_SYNCHRONIZED
	CPPUNIT_ASSERT_EQUAL((int)1, c);
}
//-----------------------------------------------------------------------------
void TestThread::testSynchronizedRecursively() {
	using namespace sambag::com;
	RecursiveMutex mutex;
	// should'nt occur a deadlock because the locking is on same thread
	callLockRecursive(mutex);
	// force lock-> two threads one lock
	bool deadlockExThrown = false;
	Deadlock dl(mutex, deadlockExThrown);
	boost::thread thread01(dl);
	boost::thread thread02(dl);
	thread01.join();
	thread02.join();
	// Deadlock Exception occurred in another thread, so we can't use
	// CPPUNIT_XXX_THROW mechanisms
	CPPUNIT_ASSERT(deadlockExThrown);
}
} //namespace
