/*
 * TestBoostTimerImpl.cpp
 *
 *  Created on: Thu Aug  9 12:35:18 2012
 *      Author: Johannes Unger
 */
#include "TestBoostTimerImpl.hpp"
#include <sambag/com/BoostTimer.hpp>
#include <boost/thread.hpp>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestBoostTimerImpl );


namespace {

typedef sambag::com::BoostTimer Timer;

void onStartTimerThread(Timer::Ptr tm) {
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	tm->start();
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	tm->start();
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	tm->start();
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	tm->start();
	boost::this_thread::sleep(boost::posix_time::millisec(500));
}

void onTimerInf(void *src, const Timer::Event &ev, int *counter) {
	++(*counter);
}

void onTimerLocked(void *src, const Timer::Event &ev) {
	boost::this_thread::sleep(boost::posix_time::millisec(5000));
}

} //namespace(s)



namespace tests {
//=============================================================================
// TestBoostTimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
void TestBoostTimerImpl::setUp() {
    sambag::com::BoostTimerImpl::startUpTimer();
}
//-----------------------------------------------------------------------------
void TestBoostTimerImpl::tearDown() {
    sambag::com::BoostTimerImpl::tearDownTimer();
}
//-----------------------------------------------------------------------------
void TestBoostTimerImpl::testMaxThreads() {
    using namespace sambag::com;
	int counter01 = 0;
	int counter02 = 0;
	boost::thread th;
	{
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Timer::Ptr timerInf = Timer::create(10);
		timerInf->setNumRepetitions(-1);
		timerInf->events::EventSender<Timer::Event>::addEventListener(
			boost::bind(&onTimerInf, _1, _2, &counter01)
		);
		timerInf->start();
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Timer::Ptr timerLocked = Timer::create(10);
		timerLocked->events::EventSender<Timer::Event>::addEventListener(
			boost::bind(&onTimerLocked, _1, _2)
		);
		timerLocked->start();
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Timer::Ptr timer01 = Timer::create(10);
		timer01->events::EventSender<Timer::Event>::addEventListener(
			boost::bind(&onTimerInf, _1, _2, &counter02)
		);
		// start thread which starts timer02 several times
		th = boost::thread(boost::bind(&onStartTimerThread, timer01));
	}
	th.join();
	CPPUNIT_ASSERT(counter01>0);
	CPPUNIT_ASSERT_EQUAL((int)4, counter02);
	CPPUNIT_ASSERT_EQUAL((int)3, sambag::com::BoostTimerImpl::getMaxNumThreads());
}
} // namespace(s)