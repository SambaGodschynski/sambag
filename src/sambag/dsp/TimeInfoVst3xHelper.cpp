/*
 * TimeInfoVst3xHelper.cpp
 *
 *  Created on: Fri Feb 13 21:11:50 2015
 *      Author: Johannes Unger
 */

#include "TimeInfoVst3xHelper.hpp"
#include <sstream>

namespace sambag { namespace dsp {
//=============================================================================
namespace timeInfoVst3xHelper {
//=============================================================================
//-----------------------------------------------------------------------------
StateAndFlags toVst3xFilter(int filter)
{
    typedef Steinberg::Vst::ProcessContext Ct;
    int f=0;
	if ((HostTimeInfo::FrxTempo & filter) > 0) {f|=Ct::kTempoValid;}
	if ((HostTimeInfo::FrxPpqPos & filter) > 0) {f|=Ct::kProjectTimeMusicValid;}
	if ((HostTimeInfo::FrxNanosValid & filter) > 0) {f|=Ct::kSystemTimeValid;}
	if ((HostTimeInfo::FrxBarsValid & filter) > 0) {f|=Ct::kBarPositionValid;}
	if ((HostTimeInfo::FrxCyclePosValid & filter) > 0) {f|=Ct::kCycleValid;}
	if ((HostTimeInfo::FrxTimeSigValid & filter) > 0) {f|=Ct::kTimeSigValid;}
	if ((HostTimeInfo::FrxSmpteValid & filter) > 0) {f|=Ct::kSmpteValid;}
	if ((HostTimeInfo::FrxClockValid & filter) > 0) {f|=Ct::kClockValid;}
    return (StateAndFlags)f;
}
//-----------------------------------------------------------------------------
HostTimeInfo::Filter toHostTimeFilter(int filter)
{
    typedef Steinberg::Vst::ProcessContext Ct;
    int f=0;
    if ((Ct::kTempoValid & filter) > 0) {f|=HostTimeInfo::FrxTempo;}
    if ((Ct::kProjectTimeMusicValid & filter) > 0) {f|=HostTimeInfo::FrxPpqPos;}
    if ((Ct::kSystemTimeValid & filter) > 0) {f|=HostTimeInfo::FrxNanosValid;}
    if ((Ct::kBarPositionValid & filter) > 0) {f|=HostTimeInfo::FrxBarsValid;}
    if ((Ct::kCycleValid & filter) > 0) {f|=HostTimeInfo::FrxCyclePosValid;}
    if ((Ct::kTimeSigValid & filter) > 0) {f|=HostTimeInfo::FrxTimeSigValid;}
    if ((Ct::kSmpteValid & filter) > 0) {f|=HostTimeInfo::FrxSmpteValid;}
    if ((Ct::kClockValid & filter) > 0) {f|=HostTimeInfo::FrxClockValid;}
    return (HostTimeInfo::Filter)f;
}
//-----------------------------------------------------------------------------
namespace {
    int getFrameRate(const Steinberg::Vst::ProcessContext &src)
    {
//    	  kVstSmpte24fps    = 0,		///< 24 fps
//        kVstSmpte25fps    = 1,		///< 25 fps
//        kVstSmpte2997fps  = 2,		///< 29.97 fps
//        kVstSmpte30fps    = 3,		///< 30 fps
//        kVstSmpte2997dfps = 4,		///< 29.97 drop
//        kVstSmpte30dfps   = 5,		///< 30 drop
//
//        kVstSmpteFilm16mm = 6, 		///< Film 16mm
//        kVstSmpteFilm35mm = 7, 		///< Film 35mm
//        kVstSmpte239fps   = 10,		///< HDTV: 23.976 fps
//        kVstSmpte249fps   = 11,		///< HDTV: 24.976 fps
//        kVstSmpte599fps   = 12,		///< HDTV: 59.94 fps
//        kVstSmpte60fps    = 13		///< HDTV: 60 fps
        // TODO: handle that drop/pull shit
        switch (src.frameRate.framesPerSecond) {
            case 24:
                return 0;
            case 25:
                return 1;
            case 30:
                return 5;
            case 60:
                return 13;
        }
        return 0;
    }
}
void convert(HostTimeInfo &dst, const Steinberg::Vst::ProcessContext &src)
{
    typedef Steinberg::Vst::ProcessContext Ct;
	dst.tempo = src.tempo;
	dst.sampleRate = src.sampleRate;
	dst.ppqPos = src.projectTimeMusic;
	dst.samplePos = src.projectTimeSamples;
	dst.nanoSeconds = (double)src.systemTime;
	dst.barStartPos = src.barPositionMusic;
	dst.cycleStartPos = src.cycleStartMusic;
	dst.cycleEndPos = src.cycleEndMusic;
	dst.timeSigNumerator = src.timeSigNumerator;
	dst.timeSigDenominator = src.timeSigDenominator;
	dst.smpteOffset = src.smpteOffsetSubframes;
	dst.smpteFrameRate = getFrameRate(src);
	dst.samplesToNextClock = src.samplesToNextClock;
	dst.transportIsChanged(false);
	dst.transportIsPlaying((src.state & Ct::kPlaying) > 0);
	dst.transportCycleIsActive((src.state & Ct::kCycleActive) > 0);
	dst.transportIsRecording((src.state & Ct::kRecording) > 0);
	dst.automationIsWriting(false);
	dst.automationIsReading(false);
}
//-----------------------------------------------------------------------------
namespace {
    Steinberg::Vst::FrameRate getFrameRate(const HostTimeInfo &src)
    {
//    	  kVstSmpte24fps    = 0,		///< 24 fps
//        kVstSmpte25fps    = 1,		///< 25 fps
//        kVstSmpte2997fps  = 2,		///< 29.97 fps
//        kVstSmpte30fps    = 3,		///< 30 fps
//        kVstSmpte2997dfps = 4,		///< 29.97 drop
//        kVstSmpte30dfps   = 5,		///< 30 drop
//
//        kVstSmpteFilm16mm = 6, 		///< Film 16mm
//        kVstSmpteFilm35mm = 7, 		///< Film 35mm
//        kVstSmpte239fps   = 10,		///< HDTV: 23.976 fps
//        kVstSmpte249fps   = 11,		///< HDTV: 24.976 fps
//        kVstSmpte599fps   = 12,		///< HDTV: 59.94 fps
//        kVstSmpte60fps    = 13		///< HDTV: 60 fps
        // TODO: handle that drop/pull shit
        Steinberg::Vst::FrameRate res = {0};
        switch (src.smpteFrameRate) {
            case 0:
                res.framesPerSecond = 24;
                break;
            case 1:
                res.framesPerSecond = 25;
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                res.framesPerSecond = 30;
                break;
            case 10:
                res.framesPerSecond = 24;
                break;
            case 11:
                res.framesPerSecond = 25;
                break;
            case 12:
            case 13:
                res.framesPerSecond = 60;
                break;
            default:
                res.framesPerSecond = 24;
                break;
        }
        return res;
    }
}
void convert(Steinberg::Vst::ProcessContext &dst, const HostTimeInfo &src)
{
    typedef Steinberg::Vst::ProcessContext Ct;
    dst.tempo = src.tempo;
	dst.sampleRate = src.sampleRate;
	dst.projectTimeMusic = src.ppqPos;
	dst.projectTimeSamples = src.samplePos;
	dst.systemTime = (Steinberg::int64)src.nanoSeconds;
	dst.barPositionMusic = src.barStartPos;
	dst.cycleStartMusic = src.cycleStartPos;
	dst.cycleEndMusic = src.cycleEndPos;
	dst.timeSigNumerator = src.timeSigNumerator;
	dst.timeSigDenominator = src.timeSigDenominator;
	dst.smpteOffsetSubframes = src.smpteOffset;
	dst.frameRate = getFrameRate(src);
	dst.samplesToNextClock = src.samplesToNextClock;
    if (src.transportIsPlaying()) { dst.state |= Ct::kPlaying; }
    if (src.transportCycleIsActive()) { dst.state |= Ct::kCycleActive; }
    if (src.transportIsRecording()) { dst.state |= Ct::kRecording; }
}
//-----------------------------------------------------------------------------
std::string toString(StateAndFlags filter)
{
    typedef Steinberg::Vst::ProcessContext Ct;
    std::stringstream ss;
    if ((Ct::kTempoValid & filter) > 0) {ss<<"Tempo ";}
    if ((Ct::kProjectTimeMusicValid  & filter) > 0) {ss<<"PpqPos ";}
    if ((Ct::kSystemTimeValid  & filter) > 0) {ss<<"NanosValid ";}
    if ((Ct::kBarPositionValid & filter) > 0) {ss<<"BarsValid ";}
    if ((Ct::kCycleValid & filter) > 0) {ss<<"CyclePosValid ";}
    if ((Ct::kTimeSigValid & filter) > 0) {ss<<"TimeSigValid ";}
    if ((Ct::kSmpteValid & filter) > 0) {ss<<"SmpteValid ";}
    if ((Ct::kClockValid & filter) > 0) {ss<<"ClockValid ";}
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);
}
//-----------------------------------------------------------------------------
std::string toString(const Steinberg::Vst::ProcessContext &dst)
{
typedef Steinberg::Vst::ProcessContext Ct;
    std::stringstream ss;
    ss<<"tempo:"<<dst.tempo<<" ";
	ss<<"sampleRate:"<<dst.sampleRate<<" ";
	ss<<"ppqPos:"<<dst.projectTimeMusic<<" ";
	ss<<"samplePos:"<<dst.projectTimeSamples<<" ";
	ss<<"nanoSeconds:"<<dst.systemTime<<" ";
	ss<<"barStartPos:"<<dst.barPositionMusic<<" ";
	ss<<"cycleStartPos:"<<dst.cycleStartMusic<<" ";
	ss<<"cycleEndPos:"<<dst.cycleEndMusic<<" ";
	ss<<"timeSigNumerator:"<<dst.timeSigNumerator<<" ";
	ss<<"timeSigDenominator:"<<dst.timeSigDenominator<<" ";
	ss<<"smpteOffset:"<<dst.smpteOffsetSubframes<<" ";
	ss<<"smpteFrameRate:"<<getFrameRate(dst)<<" ";
	ss<<"samplesToNextClock:"<<dst.samplesToNextClock<<" ";
    
    if ((dst.state & Ct::kCycleActive) > 0 ){ss<<"TransportCycleActive ";}
    if ((dst.state & Ct::kPlaying) > 0 ){ss<<"TransportPlaying ";}
    if ((dst.state & Ct::kRecording) > 0 ){ss<<"TransportRecording ";}

   
    std::string res = ss.str();
    if (res.length() == 0) {
        return "";
    }
    // remove last whitespace
    return res.erase(res.length()-1);
}

}}} // namespace(s)
