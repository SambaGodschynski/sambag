/*
 * TestTimeInfoVst3xHelper.cpp
 *
 *  Created on: Fri Feb 13 21:12:03 2015
 *      Author: Johannes Unger
 */

#include "TestTimeInfoVst3xHelper.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/dsp/HostTimeInfo.hpp>
#include <sambag/dsp/TimeInfoVst3xHelper.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestTimeInfoVst3xHelper );

namespace tests {
//=============================================================================
//  Class TestTimeInfoVst3xHelper
//=============================================================================
//-----------------------------------------------------------------------------
void TestTimeInfoVst3xHelper::testFilterConv() {
    using namespace sambag::dsp;
    using namespace sambag::dsp::timeInfoVst3xHelper;
    int f=0;
	// to vstfilter:
    f|=HostTimeInfo::FrxTempo;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo"),
        toString(toVst3xFilter(f)));
    
    f|=HostTimeInfo::FrxPpqPos;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos"),
        toString(toVst3xFilter(f)));
 
    f|=HostTimeInfo::FrxNanosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid"),
        toString(toVst3xFilter(f)));
    
    f|=HostTimeInfo::FrxBarsValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid"),
        toString(toVst3xFilter(f)));

    f|=HostTimeInfo::FrxCyclePosValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid"),
        toString(toVst3xFilter(f)));
    
    f|=HostTimeInfo::FrxTimeSigValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid"),
        toString(toVst3xFilter(f)));
  
    f|=HostTimeInfo::FrxSmpteValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid"),
        toString(toVst3xFilter(f)));
  
    f|=HostTimeInfo::FrxClockValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid ClockValid"),
        toString(toVst3xFilter(f)));

}
//-----------------------------------------------------------------------------
void TestTimeInfoVst3xHelper::testFilterConv2() {
    using namespace sambag::dsp;
    using namespace sambag::dsp::timeInfoVst3xHelper;
    typedef Steinberg::Vst::ProcessContext Ct;
    int f=0;
	// to vstfilter:
    f|=Ct::kTempoValid;
    
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo"),
        toString(toHostTimeFilter(f)));
    f|=Ct::kProjectTimeMusicValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos"),
        toString(toHostTimeFilter(f)));
    
    f|=Ct::kSystemTimeValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid"),
        toString(toHostTimeFilter(f)));

    f|=Ct::kBarPositionValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid"),
        toString(toHostTimeFilter(f)));

    f|=Ct::kCycleValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid"),
        toString(toHostTimeFilter(f)));

    f|=Ct::kTimeSigValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid"),
        toString(toHostTimeFilter(f)));

    f|=Ct::kSmpteValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid"),
        toString(toHostTimeFilter(f)));

    f|=Ct::kClockValid;
    CPPUNIT_ASSERT_EQUAL(std::string("Tempo PpqPos NanosValid BarsValid CyclePosValid TimeSigValid SmpteValid ClockValid"),
        toString(toHostTimeFilter(f)));

}
//-----------------------------------------------------------------------------
void TestTimeInfoVst3xHelper::testPODConv() {
    using namespace sambag::dsp;
    using namespace sambag::dsp::timeInfoVst3xHelper;
    
    Steinberg::Vst::ProcessContext vsttime = {0};
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
	htime.smpteFrameRate = 1; // 0 => 25 fps
	htime.samplesToNextClock = 7;
    htime.transportIsChanged(true);
    htime.transportIsPlaying(true);
    htime.transportCycleIsActive(true);
    htime.automationIsWriting(true);
    htime.automationIsReading(true);
    
    exp = "tempo:120.25 sampleRate:44100 ppqPos:10 samplePos:1100 nanoSeconds:\
110 barStartPos:13 cycleStartPos:11 cycleEndPos:14 timeSigNumerator:3 timeSigDe\
nominator:4 smpteOffset:3 smpteFrameRate:1 samplesToNextClock:7 TransportChange\
d TransportCycleActive TransportPlaying AutomationWriting AutomationReading";
    
    CPPUNIT_ASSERT_EQUAL(exp, toString(htime));
    convert(vsttime, htime);
    exp = "tempo:120.25 sampleRate:44100 ppqPos:10 samplePos:1100 nanoSeconds:\
110 barStartPos:13 cycleStartPos:11 cycleEndPos:14 timeSigNumerator:3 timeSigDe\
nominator:4 smpteOffset:3 smpteFrameRate:1 samplesToNextClock:7 TransportCycleA\
ctive TransportPlaying";
    CPPUNIT_ASSERT_EQUAL(exp, toString(vsttime));
    convert(htime, vsttime);
    CPPUNIT_ASSERT_EQUAL(exp, toString(htime));
}

} //namespace
