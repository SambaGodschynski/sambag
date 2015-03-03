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
void DefaultMidiEvents::copyFlat(IMidiEvents::Ptr _events) {
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
void DefaultMidiEvents::copyDeep(IMidiEvents::Ptr _events) {
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
//-----------------------------------------------------------------------------
void DefaultMidiEvents::insertDeep(const MidiEvent &ev) {
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

namespace {
	typedef DefaultMidiEvents::Data Data;
	template <class SrcIt, class DstIt>
	size_t copySysEx(SrcIt srcIt, DstIt dstIt, size_t size) {
		// Data status = ( *srcIt & 0xf0 ) >> 4;
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
void DefaultMidiEvents::copyDeepFiltered(IMidiEvents::Ptr _events, int channel) 
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
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
namespace {
    template<typename T>
    void veccpy(std::vector<IMidiEvents::Data> &out, const T *src, size_t num)
    {
        out.insert(out.end(), (IMidiEvents::DataPtr)src,
            (IMidiEvents::DataPtr)(src + num)
        );
    }
}
//-----------------------------------------------------------------------------
void createFlatRawData(const IMidiEvents &ev, std::vector<IMidiEvents::Data> &out)
{
    for (int i=0; i<ev.getNumEvents(); ++i) {
        IMidiEvents::ByteSize bz;
        IMidiEvents::DeltaFrames d;
        IMidiEvents::DataPtr ptr;
        boost::tie(bz, d, ptr) = ev.getMidiEvent(i);
        veccpy(out, &bz, 1);
        veccpy(out, &d, 1);
        veccpy(out, ptr, bz);
    }
}
//-----------------------------------------------------------------------------
IMidiEvents::Ptr createMidiEvents(IMidiEvents::DataPtr data, size_t byteSize) {
    IMidiEvents::DataPtr it = data;
    IMidiEvents::DataPtr end = data+byteSize;
    DefaultMidiEvents::Ptr res = DefaultMidiEvents::create();
    while(it<end) {
        size_t eventSize;
        // copy data
        IMidiEvents::MidiEvent ev;
        boost::get<0>(ev) = eventSize = *((IMidiEvents::ByteSize*)it);
        it+=sizeof(IMidiEvents::ByteSize);
        IMidiEvents::DeltaFrames d;
        boost::get<1>(ev) = d = *((IMidiEvents::DeltaFrames*)it);
        it+=sizeof(IMidiEvents::DeltaFrames);
        boost::get<2>(ev) = it;
        // insert event
        res->insertDeep(ev);
        // iterate
        it+=eventSize;
    }
    return res;
}
}} // namespace(s)
