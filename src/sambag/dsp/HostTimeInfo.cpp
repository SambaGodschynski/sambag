/*
 * HostTimeInfo.cpp
 *
 *  Created on: Thu Jan 23 12:33:01 2014
 *      Author: Johannes Unger
 */

#include "HostTimeInfo.hpp"
#include <sstream>

namespace sambag { namespace dsp {
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
    if (dst.transportIsRecording()) {ss<<"TransportRecording ";}
    if (dst.automationIsWriting()) {ss<<"AutomationWriting ";}
    if (dst.automationIsReading()) {ss<<"AutomationReading ";}
   
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);

}


}} // namespace(s)
