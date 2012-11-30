/*
 * MidiEventAdapter.hpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_VSTMIDIEVENTADAPTER_H
#define SAMBAG_VSTMIDIEVENTADAPTER_H

#include <boost/shared_ptr.hpp>
#include "IMidiEvents.hpp"
#include <aeffectx.h>
#include <sambag/com/Exception.hpp>

struct VstEventsX {
	VstInt32 	numEvents;
	VstIntPtr 	reserved;
	VstEvent  **events;
};

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class VstMidiEventAdapter.
  * TODO: impl. test
  */
struct VstMidiEventAdapter : public IMidiEvents {
//=============================================================================
	VstEventsX *events;
	bool ownerOfData;
	VstMidiEventAdapter(VstEventsX *events) : 
		events(events), ownerOfData(false)
	{
	}
	VstMidiEventAdapter(const VstMidiEventAdapter &b) {
		events = b.events;
		ownerOfData = false;
	}
	virtual Int getNumEvents() const 
	{
		return events->numEvents;
	}
	virtual MidiEvent getMidiEvent(Int index) const
	{
		//boost::tuple<ByteSize, DeltaFrames, DataPtr> 
		MidiEvent res;
		VstEvent *ev = events->events[index];
		if (ev->type != kVstMidiType) {
			boost::get<0>(res) = 0;
			return res;
		}
		boost::get<0>(res) = ev->byteSize;
		boost::get<1>(res) = ev->deltaFrames;
		boost::get<2>(res) = (DataPtr) &(ev->data);
		return res;
	}
	VstMidiEventAdapter(IMidiEvents *ev) : ownerOfData(false)
	{
		VstMidiEventAdapter *_ev = 
			dynamic_cast<VstMidiEventAdapter*>(ev);
		if (_ev) {
			this->events = _ev->events;
			return;
		}
		// create new data
		ownerOfData = true;
		events = new VstEventsX();
		events->numEvents = ev->getNumEvents();
		events->events = new VstEvent*[events->numEvents];
		for (Int i=0; i<events->numEvents; ++i) {
			MidiEvent tmp = ev->getMidiEvent(i);
			events->events[i] = new VstEvent();
			events->events[i]->type = kVstMidiType;
			events->events[i]->byteSize = boost::get<0>(tmp);
			events->events[i]->deltaFrames = boost::get<1>(tmp);
			events->events[i]->flags = 0;
			DataPtr bytes = boost::get<2>(tmp);
			SAMBAG_ASSERT(events->events[i]->byteSize < 16);
			for (Int j=0; j<events->events[i]->byteSize; ++j) {
				events->events[i]->data[j] = bytes[j];
			}
		}
	}
	virtual ~VstMidiEventAdapter() {
		if (!ownerOfData)
			return;
		for (Int i=0; i<events->numEvents; ++i) {
			delete events->events[i];
		}
		delete[] events->events;
		delete events;
	}

}; // VstMidiEventAdapter
}} // namespace(s)

#endif /* SAMBAG_VSTMIDIEVENTADAPTER_H */
