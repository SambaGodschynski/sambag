/*
 * TestBoostTimerImpl.cpp
 *
 *  Created on: Thu Aug  9 12:35:18 2012
 *      Author: Johannes Unger
 */

#include "TestBoostTimerImpl.hpp"
#include <sambag/disco/components/Timer.hpp>
#include <sambag/disco/components/windowImpl/BoostTimerImpl.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/disco/components/windowImpl/WindowTestToolkit.hpp>
#include <boost/thread.hpp>
#include "TestComponents.hpp"
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestBoostTimerImpl );


void onTimerInf(int *counter) {
	++(*counter);
}

void onStartTimerThread(sambag::disco::components::Timer::Ptr tm) {
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

namespace tests {
sambag::disco::components::WindowTestToolkit<>::Ptr wfac;
//=============================================================================
// TestBoostTimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
void TestBoostTimerImpl::setUp() {
	using namespace sambag::disco::components;
	if (!wfac)
		wfac = WindowTestToolkit<>::create();
	sambag::disco::components::setWindowToolkit(wfac.get());
	wfac->setMainLoopWaiting(6000);
}
//-----------------------------------------------------------------------------
void TestBoostTimerImpl::tearDown() {
	sambag::disco::components::setWindowToolkit(NULL);
}
//-----------------------------------------------------------------------------
void TestBoostTimerImpl::testMaxThreads() {
	using namespace sambag::disco::components;
	Timer::Ptr timerInf = Timer::create(10);
	timerInf->setNumRepetitions(-1);
	int counter01 = 0;
	timerInf->EventSender<Timer::Event>::addEventListener(
		boost::bind(&onTimerInf, &counter01)
	);
	timerInf->start();
	Timer::Ptr timer01 = Timer::create(10);
	int counter02 = 0;
	timer01->EventSender<Timer::Event>::addEventListener(
		boost::bind(&onTimerInf, &counter02)
	);
	// start thread which starts timer02 several times
	boost::thread th(boost::bind(&onStartTimerThread, timer01));
	getWindowToolkit()->startMainLoop();
	th.join();
	CPPUNIT_ASSERT(counter01>0);
	CPPUNIT_ASSERT_EQUAL((int)4, counter02);
	CPPUNIT_ASSERT_EQUAL((int)2, BoostTimerImpl::getMaxNumThreads());
}
} // namespace(s)
