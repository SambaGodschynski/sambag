/*
 * VstMidiEventAdapter.cpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#include "VstMidiEventAdapter.hpp"
namespace sambag { namespace dsp {

namespace {
	typedef IMidiEvents::Data Data;
	VstEvents * allocVstEvents(size_t numEvents) {
		size_t size = sizeof(VstEvents) + (numEvents * sizeof(VstEvent*));
		size += numEvents * sizeof(VstEvent);
		VstEvents *res = (VstEvents*)new Data[size];
		memset(res, 0, size);
		return res;
	}
	VstEvent * allocVstEvent(size_t bytes) {
		size_t size = sizeof(VstEvent) + (bytes * sizeof(Data*));
		size += (bytes) * sizeof(Data);
		VstEvent * res = (VstEvent*)new Data[size];
		return res;
	}
} // namespace 

//=============================================================================
// class VstMidiEventAdapter.
//=============================================================================
//-----------------------------------------------------------------------------
void VstMidiEventAdapter::allocDataIfNecessary(IMidiEvents *ev) {
	if (!ownerOfData || !events) {
		events = allocVstEvents(ev->getNumEvents());
		maxEvents = ev->getNumEvents();
		ownerOfData = true;
		return;
	}
	
	if (maxEvents < ev->getNumEvents()) {
		freeDataIfNecessary();
		events = allocVstEvents(ev->getNumEvents());
		maxEvents = ev->getNumEvents();
		return;
	}
}
//-----------------------------------------------------------------------------
VstEvent * VstMidiEventAdapter::allocVstEventNecessary(size_t bytes, VstEvent *old) 
{
	if (!old) {
		return allocVstEvent(bytes);
	}
	if (old->byteSize < bytes) {
		delete old;
		return allocVstEvent(bytes);
	}
	return old;
}
//-----------------------------------------------------------------------------
void VstMidiEventAdapter::set(IMidiEvents *ev) {
	// special case:
	VstMidiEventAdapter *_ev = 
		dynamic_cast<VstMidiEventAdapter*>(ev);
	if (_ev) {
		freeDataIfNecessary();
		this->events = _ev->events;
		ownerOfData = false;
		return;
	}
	allocDataIfNecessary(ev);
	events->numEvents = ev->getNumEvents();
	for (Int i=0; i<events->numEvents; ++i) {
		MidiEvent tmp = ev->getMidiEvent(i);
		events->events[i] = 
			allocVstEventNecessary(boost::get<0>(tmp), events->events[i]);
		events->events[i]->type = kVstMidiType;
		events->events[i]->byteSize = boost::get<0>(tmp);
		events->events[i]->deltaFrames = boost::get<1>(tmp);
		events->events[i]->flags = 0;
		DataPtr bytes = boost::get<2>(tmp);
		/*for (Int j=0; j<events->events[i]->byteSize; ++j) {
			events->events[i]->data[j] = bytes[j];
		}*/
		memcpy(events->events[i]->data, bytes, events->events[i]->byteSize);
	}
}
//-----------------------------------------------------------------------------
void VstMidiEventAdapter::freeDataIfNecessary() {
	if (!ownerOfData) {
		return;
	}
	if (!events)
		return;
	for (Int i=0; i<maxEvents; ++i) {
		delete events->events[i];
	}
	delete events;
	events = NULL;
}
//-----------------------------------------------------------------------------
VstMidiEventAdapter::VstMidiEventAdapter(IMidiEvents *ev) : 
	events(NULL), ownerOfData(false) 
{
	set(ev);
}
//-----------------------------------------------------------------------------
VstMidiEventAdapter::~VstMidiEventAdapter() {
	freeDataIfNecessary();
}
}} // namespace(s)
