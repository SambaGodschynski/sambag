/*
 * TestBoostTimer2.cpp
 *
 *  Created on: Sun Jul 14 17:01:01 2013
 *      Author: Johannes Unger
 */

#include "TestBoostTimer2.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/BoostTimer2.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestBoostTimer2 );

namespace {

typedef sambag::com::BoostTimer2 Timer;

void onTimerInf(void *src, const Timer::Event &ev, int *counter) {
	++(*counter);
    std::cout<<"."<<std::flush;
}

} //namespace(s)



namespace tests {
//=============================================================================
// TestBoostTimerImpl
//=============================================================================
//-----------------------------------------------------------------------------
void TestBoostTimer2::setUp() {
}
//-----------------------------------------------------------------------------
void TestBoostTimer2::tearDown() {
}
//-----------------------------------------------------------------------------
void TestBoostTimer2::testStartTimer() {
    using namespace sambag::com;
	int counter01 = 0;
    {
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Timer::Ptr timerInf = Timer::create(1000);
        timerInf->setNumRepetitions(3);
        timerInf->events::EventSender<Timer::Event>::addEventListener(
            boost::bind(&onTimerInf, _1, _2, &counter01)
        );
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(5000));
        timerInf->stop();
    } 
	CPPUNIT_ASSERT_EQUAL(3, counter01);
}
//-----------------------------------------------------------------------------
void TestBoostTimer2::testStopTimer() {
    using namespace sambag::com;
	int counter01 = 0;
     {
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Timer::Ptr timerInf = Timer::create(1000);
        timerInf->setNumRepetitions(10);
        timerInf->events::EventSender<Timer::Event>::addEventListener(
            boost::bind(&onTimerInf, _1, _2, &counter01)
        );
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(2500));
        timerInf->stop();
        boost::this_thread::sleep(boost::posix_time::millisec(2000));
    } 
	CPPUNIT_ASSERT_EQUAL(2, counter01);


}
//-----------------------------------------------------------------------------
void TestBoostTimer2::testRestartTimer() {
    using namespace sambag::com;
	int counter01 = 0;
    {
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Timer::Ptr timerInf = Timer::create(1000);
        timerInf->setNumRepetitions(3);
        timerInf->events::EventSender<Timer::Event>::addEventListener(
            boost::bind(&onTimerInf, _1, _2, &counter01)
        );
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(5000));
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(5000));
    } 
	CPPUNIT_ASSERT_EQUAL(6, counter01);
    
    counter01 = 0;
    {
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Timer::Ptr timerInf = Timer::create(1000);
        timerInf->setNumRepetitions(3);
        timerInf->events::EventSender<Timer::Event>::addEventListener(
            boost::bind(&onTimerInf, _1, _2, &counter01)
        );
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(3500));
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(3500));
    } 
	CPPUNIT_ASSERT_EQUAL(6, counter01);

    counter01 = 0;
    {
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Timer::Ptr timerInf = Timer::create(1000);
        timerInf->setNumRepetitions(-1);
        timerInf->events::EventSender<Timer::Event>::addEventListener(
            boost::bind(&onTimerInf, _1, _2, &counter01)
        );
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(3500));
        timerInf->stop();
        timerInf->start();
        boost::this_thread::sleep(boost::posix_time::millisec(3500));
        timerInf->stop();
    } 
	CPPUNIT_ASSERT_EQUAL(6, counter01);


}
//-----------------------------------------------------------------------------
void TestBoostTimer2::testFailure() {
    Timer::Ptr timerInf = Timer::create(1000);
    timerInf->setNumRepetitions(-1);
    timerInf->start();
}


} // namespace(s)