/*
 * TimeInfoVst2xHelper.hpp
 *
 *  Created on: Thu Jan 23 12:33:01 2014
 *      Author: Johannes Unger
 */

#include "TimeInfoVst2xHelper.hpp"
#include <sstream>

namespace sambag { namespace dsp {
//=============================================================================
namespace timeInfoVst2xHelper {
//=============================================================================
//-----------------------------------------------------------------------------
VstTimeInfoFlags toVst2xFilter(int filter) {
    int f=0;
	if ((HostTimeInfo::FrxTempo & filter) > 0) {f|=kVstTempoValid;}
	if ((HostTimeInfo::FrxPpqPos & filter) > 0) {f|=kVstPpqPosValid;}
	if ((HostTimeInfo::FrxNanosValid & filter) > 0) {f|=kVstNanosValid;}
	if ((HostTimeInfo::FrxBarsValid & filter) > 0) {f|=kVstBarsValid;}
	if ((HostTimeInfo::FrxCyclePosValid & filter) > 0) {f|=kVstCyclePosValid;}
	if ((HostTimeInfo::FrxTimeSigValid & filter) > 0) {f|=kVstTimeSigValid;}
	if ((HostTimeInfo::FrxSmpteValid & filter) > 0) {f|=kVstSmpteValid;}
	if ((HostTimeInfo::FrxClockValid & filter) > 0) {f|=kVstClockValid;} 
    return (VstTimeInfoFlags)f;
}
//-----------------------------------------------------------------------------
HostTimeInfo::Filter toHostTimeFilter(int filter)  {
    int f=0;
    if ((kVstTempoValid & filter) > 0) {f|=HostTimeInfo::FrxTempo;}
    if ((kVstPpqPosValid & filter) > 0) {f|=HostTimeInfo::FrxPpqPos;}
    if ((kVstNanosValid & filter) > 0) {f|=HostTimeInfo::FrxNanosValid;}
    if ((kVstBarsValid & filter) > 0) {f|=HostTimeInfo::FrxBarsValid;}
    if ((kVstCyclePosValid & filter) > 0) {f|=HostTimeInfo::FrxCyclePosValid;}
    if ((kVstTimeSigValid & filter) > 0) {f|=HostTimeInfo::FrxTimeSigValid;}
    if ((kVstSmpteValid & filter) > 0) {f|=HostTimeInfo::FrxSmpteValid;}
    if ((kVstClockValid & filter) > 0) {f|=HostTimeInfo::FrxClockValid;} 
    return (HostTimeInfo::Filter)f;
}
//-----------------------------------------------------------------------------
void convert(HostTimeInfo &dst, const VstTimeInfo &src) {
	dst.tempo = src.tempo;
	dst.sampleRate = src.sampleRate;
	dst.ppqPos = src.ppqPos;
	dst.samplePos = src.samplePos;
	dst.nanoSeconds = src.nanoSeconds;
	dst.barStartPos = src.barStartPos;
	dst.cycleStartPos = src.cycleStartPos;
	dst.cycleEndPos = src.cycleEndPos;
	dst.timeSigNumerator = src.timeSigNumerator;
	dst.timeSigDenominator = src.timeSigDenominator;
	dst.smpteOffset = src.smpteOffset;
	dst.smpteFrameRate = src.smpteFrameRate;
	dst.samplesToNextClock = src.samplesToNextClock;
	dst.transportIsChanged((src.flags & kVstTransportChanged) > 0);
	dst.transportIsPlaying((src.flags & kVstTransportPlaying) > 0);
	dst.transportCycleIsActive((src.flags & kVstTransportCycleActive) > 0);
	dst.transportIsRecording((src.flags & kVstTransportRecording) > 0);
	dst.automationIsWriting((src.flags & kVstAutomationWriting) > 0);
	dst.automationIsReading((src.flags & kVstAutomationReading) > 0);
}
//-----------------------------------------------------------------------------
void convert(VstTimeInfo &dst, const HostTimeInfo &src) {
    dst.tempo = src.tempo;
	dst.sampleRate = src.sampleRate;
	dst.ppqPos = src.ppqPos;
	dst.samplePos = src.samplePos;
	dst.nanoSeconds = src.nanoSeconds;
	dst.barStartPos = src.barStartPos;
	dst.cycleStartPos = src.cycleStartPos;
	dst.cycleEndPos = src.cycleEndPos;
	dst.timeSigNumerator = src.timeSigNumerator;
	dst.timeSigDenominator = src.timeSigDenominator;
	dst.smpteOffset = src.smpteOffset;
	dst.smpteFrameRate = src.smpteFrameRate;
	dst.samplesToNextClock = src.samplesToNextClock;
    if (src.transportIsChanged()) { dst.flags |= kVstTransportChanged; }
    if (src.transportIsPlaying()) { dst.flags |= kVstTransportPlaying; }
    if (src.transportCycleIsActive()) { dst.flags |= kVstTransportCycleActive; }
    if (src.automationIsWriting()) { dst.flags |= kVstAutomationWriting; }
    if (src.automationIsReading()) { dst.flags |= kVstAutomationReading; }
}
//-----------------------------------------------------------------------------
std::string toString(VstTimeInfoFlags filter) {
    std::stringstream ss;
    if ((kVstTempoValid & filter) > 0) {ss<<"Tempo ";}
    if ((kVstPpqPosValid & filter) > 0) {ss<<"PpqPos ";}
    if ((kVstNanosValid & filter) > 0) {ss<<"NanosValid ";}
    if ((kVstBarsValid & filter) > 0) {ss<<"BarsValid ";}
    if ((kVstCyclePosValid & filter) > 0) {ss<<"CyclePosValid ";}
    if ((kVstTimeSigValid & filter) > 0) {ss<<"TimeSigValid ";}
    if ((kVstSmpteValid & filter) > 0) {ss<<"SmpteValid ";}
    if ((kVstClockValid & filter) > 0) {ss<<"ClockValid ";}
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);
}
//-----------------------------------------------------------------------------
std::string toString(HostTimeInfo::Filter filter) {
    std::stringstream ss;
    if ((HostTimeInfo::FrxTempo & filter) > 0) {ss<<"Tempo ";}
	if ((HostTimeInfo::FrxPpqPos & filter) > 0) {ss<<"PpqPos ";}
	if ((HostTimeInfo::FrxNanosValid & filter) > 0) {ss<<"NanosValid ";}
	if ((HostTimeInfo::FrxBarsValid & filter) > 0) {ss<<"BarsValid ";}
	if ((HostTimeInfo::FrxCyclePosValid & filter) > 0) {ss<<"CyclePosValid ";}
	if ((HostTimeInfo::FrxTimeSigValid & filter) > 0) {ss<<"TimeSigValid ";}
	if ((HostTimeInfo::FrxSmpteValid & filter) > 0) {ss<<"SmpteValid ";}
	if ((HostTimeInfo::FrxClockValid & filter) > 0) {ss<<"ClockValid ";}
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);
}
//-----------------------------------------------------------------------------
std::string toString(const VstTimeInfo &dst) {
    std::stringstream ss;
    ss<<"tempo:"<<dst.tempo<<" ";
	ss<<"sampleRate:"<<dst.sampleRate<<" ";
	ss<<"ppqPos:"<<dst.ppqPos<<" ";
	ss<<"samplePos:"<<dst.samplePos<<" ";
	ss<<"nanoSeconds:"<<dst.nanoSeconds<<" ";
	ss<<"barStartPos:"<<dst.barStartPos<<" ";
	ss<<"cycleStartPos:"<<dst.cycleStartPos<<" ";
	ss<<"cycleEndPos:"<<dst.cycleEndPos<<" ";
	ss<<"timeSigNumerator:"<<dst.timeSigNumerator<<" ";
	ss<<"timeSigDenominator:"<<dst.timeSigDenominator<<" ";
	ss<<"smpteOffset:"<<dst.smpteOffset<<" ";
	ss<<"smpteFrameRate:"<<dst.smpteFrameRate<<" ";
	ss<<"samplesToNextClock:"<<dst.samplesToNextClock<<" ";
    
    if ((dst.flags & kVstTransportChanged) > 0 ){ss<<"TransportChanged ";}
    if ((dst.flags & kVstTransportCycleActive) > 0 ){ss<<"TransportCycleActive ";}
    if ((dst.flags & kVstTransportPlaying) > 0 ){ss<<"TransportPlaying ";}
    if ((dst.flags & kVstAutomationWriting) > 0 ){ss<<"AutomationWriting ";}
    if ((dst.flags & kVstAutomationReading) > 0 ){ss<<"AutomationReading ";}
   
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);
}
//-----------------------------------------------------------------------------
std::string toString(const HostTimeInfo &dst) {
    std::stringstream ss;
    ss<<"tempo:"<<dst.tempo<<" ";
	ss<<"sampleRate:"<<dst.sampleRate<<" ";
	ss<<"ppqPos:"<<dst.ppqPos<<" ";
	ss<<"samplePos:"<<dst.samplePos<<" ";
	ss<<"nanoSeconds:"<<dst.nanoSeconds<<" ";
	ss<<"barStartPos:"<<dst.barStartPos<<" ";
	ss<<"cycleStartPos:"<<dst.cycleStartPos<<" ";
	ss<<"cycleEndPos:"<<dst.cycleEndPos<<" ";
	ss<<"timeSigNumerator:"<<dst.timeSigNumerator<<" ";
	ss<<"timeSigDenominator:"<<dst.timeSigDenominator<<" ";
	ss<<"smpteOffset:"<<dst.smpteOffset<<" ";
	ss<<"smpteFrameRate:"<<dst.smpteFrameRate<<" ";
	ss<<"samplesToNextClock:"<<dst.samplesToNextClock<<" ";
    
    if (dst.transportIsChanged()) {ss<<"TransportChanged ";}
    if (dst.transportCycleIsActive()) {ss<<"TransportCycleActive ";}
    if (dst.transportIsPlaying()) {ss<<"TransportPlaying ";}
    if (dst.automationIsWriting()) {ss<<"AutomationWriting ";}
    if (dst.automationIsReading()) {ss<<"AutomationReading ";}
   
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);

}
} // timeInfoVst2xHelper
}} // namespace(s)
