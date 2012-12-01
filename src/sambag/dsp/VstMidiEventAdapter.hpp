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

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class VstMidiEventAdapter.
  */
struct VstMidiEventAdapter : public IMidiEvents {
//=============================================================================
	VstEvents *events;
	bool ownerOfData;
	//-------------------------------------------------------------------------
	VstMidiEventAdapter(VstEvents *events) : 
		events(events), ownerOfData(false) {
	}
	//-------------------------------------------------------------------------
	VstMidiEventAdapter(const VstMidiEventAdapter &b) {
		events = b.events;
		ownerOfData = false;
	}
	//-------------------------------------------------------------------------
	VstEvents * allocVstEventsFor(IMidiEvents *ev);
	//-------------------------------------------------------------------------
	VstEvent * allocVstEventFor(size_t bytes);
	//-------------------------------------------------------------------------
	virtual Int getNumEvents() const {
		return events->numEvents;
	}
	//-------------------------------------------------------------------------
	virtual MidiEvent getMidiEvent(Int index) const {
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
	//-------------------------------------------------------------------------
	VstMidiEventAdapter(IMidiEvents *ev);
	//-------------------------------------------------------------------------
	virtual ~VstMidiEventAdapter();

}; // VstMidiEventAdapter
}} // namespace(s)

#endif /* SAMBAG_VSTMIDIEVENTADAPTER_H */
