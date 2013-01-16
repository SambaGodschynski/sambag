/*
 * DefautMidiEvents.cpp
 *
 *  Created on: Fri Nov 30 13:03:16 2012
 *      Author: Johannes Unger
 */

#include "DefaultMidiEvents.hpp"

namespace sambag { namespace dsp {
//=============================================================================
//  Class DefaultMidiEvents
//=============================================================================
//-----------------------------------------------------------------------------
void DefaultMidiEvents::copyFlat(IMidiEvents *_events) {
	events.clear();
	events.reserve(_events->getNumEvents());
	int num = _events->getNumEvents();
	for (int i=0; i<num; ++i) {
		events.push_back(
			_events->getMidiEvent(i)
		);
	}
}
//-----------------------------------------------------------------------------
void DefaultMidiEvents::copyDeep(IMidiEvents *_events) {
	events.clear();
	events.reserve(_events->getNumEvents());
	dataContainer.reserve(_events->getNumEvents());
	int num = _events->getNumEvents();
	for (int i=0; i<num; ++i) {
		MidiEvent ev = _events->getMidiEvent(i);
		ByteSize bytes = boost::get<0>(ev);
		DeltaFrames d = boost::get<1>(ev);
		DataPtr srcdata = boost::get<2>(ev);
		dataContainer.push_back(DataArray(new Data[bytes]));
		DataPtr dstdata = dataContainer.back().get();
		for (int j=0; j<bytes; ++j) {
			dstdata[j] = srcdata[j];
		}
		events.push_back(
			MidiEvent(bytes, d, dstdata)
		);
	}
}
namespace {
	typedef DefaultMidiEvents::Data Data;
	template <class SrcIt, class DstIt>
	size_t copySysEx(SrcIt srcIt, DstIt dstIt, size_t size) {
		Data status = ( *srcIt & 0xf0 ) >> 4;
		if (*srcIt!=0xf0) {
			return 0;
		}
		*dstIt = *srcIt;
		size_t i=1;
		for (; i<size; ++i) {
			*(++dstIt) = *(++srcIt);
			if (*srcIt==0xf7) {
				*(++dstIt) = *(++srcIt);
				return i+1;
			}
		}
		return i;
	}
} // namespace(s)
//-----------------------------------------------------------------------------
void DefaultMidiEvents::copyDeepFiltered(IMidiEvents *_events, int channel) 
{
	events.clear();
	events.reserve(_events->getNumEvents());
	dataContainer.reserve(_events->getNumEvents());
	int num = _events->getNumEvents();
	for (int i=0; i<num; ++i) {
		MidiEvent ev = _events->getMidiEvent(i);
		ByteSize bytes = boost::get<0>(ev);
		DeltaFrames d = boost::get<1>(ev);
		DataPtr srcdata = boost::get<2>(ev);
		dataContainer.push_back(DataArray(new Data[bytes]));
		DataPtr dstdata = dataContainer.back().get();
		int wrote = 0;
		for (int j=0; j<bytes;) {
			if (srcdata[j] == 0xf0 ) { //sysex
				size_t numCopied = copySysEx(&srcdata[j], &dstdata[wrote], bytes-j);
				j+=numCopied;
				wrote+=numCopied;
				continue;
			}
			Data status = ( srcdata[j] & 0xf0 ) >> 4;
			if (status>=0x8 && status<0xF) { // check message range
				// get channel
				Data n = srcdata[j] & 0xf;	
				if (n != channel) {
					j+=3; // status + data1 + data2
					continue;
				}
				if (j+2>=bytes) {
					break;
				}
				dstdata[wrote++] = srcdata[j++]; // status byte
				dstdata[wrote++] = srcdata[j++]; // data 1
				dstdata[wrote++] = srcdata[j++]; // data 2
				continue;
			} else {		
				dstdata[wrote++] = srcdata[j++];
			}
		}
		
		events.push_back(
			MidiEvent(wrote, d, dstdata)
		);
	}
}
}} // namespace(s)
