/*
 * TestTimeInfoVst2xHelper.cpp
 *
 *  Created on: Thu Jan 23 14:21:06 2014
 *      Author: Johannes Unger
 */

#include "TestTimeInfoVst2xHelper.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/dsp/HostTimeInfo.hpp>
#include <sambag/dsp/TimeInfoVst2xHelper.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestTimeInfoVst2xHelper );

namespace tests {
//=============================================================================
//  Class TestTimeInfoVst2xHelper
//=============================================================================
//-----------------------------------------------------------------------------
void TestTimeInfoVst2xHelper::testFilterConv() {
    using namespace sambag::dsp;
    using namespace sambag::dsp::timeInfoVst2xHelper;
    int f=0;
	// to vstfilter:
    f|=HostTimeInfo::FrxTempo;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo"),
        toString(toVst2xFilter(f)));
    
    f|=HostTimeInfo::FrxPpqPos;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos"),
        toString(toVst2xFilter(f)));
 
    f|=HostTimeInfo::FrxNanosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid"),
        toString(toVst2xFilter(f)));
    
    f|=HostTimeInfo::FrxBarsValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid"),
        toString(toVst2xFilter(f)));

    f|=HostTimeInfo::FrxCyclePosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid"),
        toString(toVst2xFilter(f)));
    
    f|=HostTimeInfo::FrxTimeSigValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid"),
        toString(toVst2xFilter(f)));
  
    f|=HostTimeInfo::FrxSmpteValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid"),
        toString(toVst2xFilter(f)));
  
    f|=HostTimeInfo::FrxClockValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid ClockValid"),
        toString(toVst2xFilter(f)));

}
//-----------------------------------------------------------------------------
void TestTimeInfoVst2xHelper::testFilterConv2() {
    using namespace sambag::dsp;
    using namespace sambag::dsp::timeInfoVst2xHelper;
    int f=0;
	// to vstfilter:
    f|=kVstTempoValid;
    
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo"),
        toString(toHostTimeFilter(f)));
    f|=kVstPpqPosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos"),
        toString(toHostTimeFilter(f)));
    
    f|=kVstNanosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid"),
        toString(toHostTimeFilter(f)));

    f|=kVstBarsValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid"),
        toString(toHostTimeFilter(f)));

    f|=kVstCyclePosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid"),
        toString(toHostTimeFilter(f)));

    f|=kVstTimeSigValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid"),
        toString(toHostTimeFilter(f)));

    f|=kVstSmpteValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid"),
        toString(toHostTimeFilter(f)));

    f|=kVstClockValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid ClockValid"),
        toString(toHostTimeFilter(f)));

}
//-----------------------------------------------------------------------------
void TestTimeInfoVst2xHelper::testPODConv() {
    using namespace sambag::dsp;
    using namespace sambag::dsp::timeInfoVst2xHelper;
    
    VstTimeInfo vsttime = {0};
    HostTimeInfo htime;
    std::string exp = "tempo:0 sampleRate:0 ppqPos:0 samplePos:0 nanoSeconds:0 barStartPos\
:0 cycleStartPos:0 cycleEndPos:0 timeSigNumerator:0 timeSigDenominator:0 smpteO\
ffset:0 smpteFrameRate:0 samplesToNextClock:0";
    CPPUNIT_ASSERT_EQUAL(exp, toString(vsttime));
    convert(htime, vsttime);
    CPPUNIT_ASSERT_EQUAL(exp, toString(htime));
    
    htime.tempo=120.25;
    htime.sampleRate=44100.;
	htime.ppqPos = 10.;
	htime.samplePos = 1100.;
	htime.nanoSeconds = 110.;
	htime.barStartPos = 13.;
	htime.cycleStartPos = 11.;
	htime.cycleEndPos = 14.;
	htime.timeSigNumerator = 3;
	htime.timeSigDenominator = 4;
	htime.smpteOffset = 3;
	htime.smpteFrameRate = 22;
	htime.samplesToNextClock = 7;
    htime.transportIsChanged(true);
    htime.transportIsPlaying(true);
    htime.transportCycleIsActive(true);
    htime.automationIsWriting(true);
    htime.automationIsReading(true);
    
    exp = "tempo:120.25 sampleRate:44100 ppqPos:10 samplePos:1100 nanoSeconds:\
110 barStartPos:13 cycleStartPos:11 cycleEndPos:14 timeSigNumerator:3 timeSigDe\
nominator:4 smpteOffset:3 smpteFrameRate:22 samplesToNextClock:7 TransportChang\
ed TransportCycleActive TransportPlaying AutomationWriting AutomationReading";
    
    CPPUNIT_ASSERT_EQUAL(exp, toString(htime));
    convert(vsttime, htime);
    CPPUNIT_ASSERT_EQUAL(exp, toString(vsttime));
    convert(htime, vsttime);
    CPPUNIT_ASSERT_EQUAL(exp, toString(htime));
}
} //namespace
