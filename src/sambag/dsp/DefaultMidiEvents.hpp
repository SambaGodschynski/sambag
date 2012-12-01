/*
 * DefautMidiEvents.hpp
 *
 *  Created on: Fri Nov 30 13:03:16 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTMIDIEVENTS_H
#define SAMBAG_DEFAULTMIDIEVENTS_H

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "IMidiEvents.hpp"
#include <vector>

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class DefaultMidiEvents.
  */
struct DefaultMidiEvents : public IMidiEvents {
//=============================================================================
	std::vector<MidiEvent> events;
	typedef boost::shared_array<Data> DataArray;
	std::vector<DataArray> dataContainer; // needed for deep copy
	DefaultMidiEvents(IMidiEvents *_events = NULL) 
	{
		if (_events) {
			copyFlat(_events);
		}
	}
	virtual Int getNumEvents() const 
	{
		return events.size();
	}
	virtual MidiEvent getMidiEvent(Int index) const
	{
		return events[index];
	}
	/**
	 * flat copy of midi data. (remember: MidiEvent contains midi data as ptr)
	 * @note: clears all previous setted data
	 */
	void copyFlat(IMidiEvents *_events) 
	{
		events.clear();
		events.reserve(_events->getNumEvents());
		int num = _events->getNumEvents();
		for (int i=0; i<num; ++i) {
			events.push_back(
				_events->getMidiEvent(i)
			);
		}
	}
	/**
	 * deep copy of midi data.
	 * @note: clears all previous setted data
	 */
	void copyDeep(IMidiEvents *_events) 
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
			for (int j=0; j<bytes; ++j) {
				dstdata[j] = srcdata[j];
			}
			events.push_back(
				MidiEvent(bytes, d, dstdata)
			);
		}
	}
}; // DefaultMidiEvents
}} // namespace(s)

#endif /* SAMBAG_DEFAULTMIDIEVENTS_H */
