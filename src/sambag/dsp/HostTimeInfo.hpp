/*
 * HostTimeInfo.hpp
 *
 *  Created on: Thu Oct 18 10:35:04 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_HOSTTIMEINFO_H
#define SAMBAG_HOSTTIMEINFO_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class HostTimeInfo.
  */
struct HostTimeInfo {
//=============================================================================
	enum Filter {
		FrxTempo = 1,
		FrxPpqPos = 1<<1,
		FrxNanosValid = 1<<2,
		FrxBarsValid = 1<<3,
		FrxCyclePosValid = 1<<4,
		FrxTimeSigValid = 1<<5,
		FrxSmpteValid = 1<<6,
		FrxClockValid = 1<<7
	};
	double tempo;
	double sampleRate; // always valid
	double ppqPos; // Musical Position, in Quarter Note (1.0 equals 1 Quarter Note).
	bool transportIsPlaying;
	double 	samplePos; // current Position in audio samples (always valid)
	double 	nanoSeconds; //System Time in nanoseconds (10^-9 second).
	double barStartPos; // last Bar Start Position, in Quarter Note
	double cycleStartPos; // Cycle Start (left locator), in Quarter Note.
	double cycleEndPos; // Cycle End (right locator), in Quarter Note.
	int timeSigNumerator; // Time Signature Numerator (e.g. 3 for 3/4).
	int timeSigDenominator; // Time Signature Denominator (e.g. 4 for 3/4).
	int smpteOffset; // SMPTE offset (in SMPTE subframes (bits; 1/80 of a frame)).
	int smpteFrameRate;
	int samplesToNextClock; // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest clock). 
	bool transportIsChanged; // 	indicates that play, cycle or record state has changed 
	bool transportCycleIsActive; // set if Host sequencer is in cycle mode
	bool transportIsRecording; // set if Host sequencer is in record mode
	bool automationIsWriting; // set if automation write mode active (record parameter changes)
	bool automationIsReading; // set if automation read mode active (play parameter changes) 
	HostTimeInfo() :
		tempo(0.),
		sampleRate(0.),
		ppqPos(0.),
		transportIsPlaying(false),
		samplePos(0.),
		nanoSeconds(0.),
		barStartPos(0.),
		cycleStartPos(0.),
		cycleEndPos(0.),
		timeSigNumerator(0),
		timeSigDenominator(0),
		smpteOffset(0),
		smpteFrameRate(0),
		samplesToNextClock(0),
		transportIsChanged(false),
		transportCycleIsActive(false),
		transportIsRecording(false),
		automationIsWriting(false),
		automationIsReading(false)
	{
	}
};
}} // namespace(s)

#endif /* SAMBAG_HOSTTIMEINFO_H */
