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
	//-------------------------------------------------------------------------
	DefaultMidiEvents(IMidiEvents *_events = NULL) {
		if (_events) {
			copyFlat(_events);
		}
	}
	//-------------------------------------------------------------------------
	virtual Int getNumEvents() const {
		return events.size();
	}
	//-------------------------------------------------------------------------
	virtual MidiEvent getMidiEvent(Int index) const {
		return events[index];
	}
	//-------------------------------------------------------------------------
	/**
	 * @brief flat copy of midi data. (remember: MidiEvent contains midi data as ptr)
	 * @note: clears all previous setted data
	 */
	void copyFlat(IMidiEvents *_events);
	//-------------------------------------------------------------------------
	/**
     * @brief deep copy of midi data.
	 * @note: clears all previous setted data
	 */
	void copyDeep(IMidiEvents *_events);
	//-------------------------------------------------------------------------
	/**
    * @brief deep copy of midi data using a channel filter.
	 * all channel related messages wich not belong to filterChannel will be
	 * ignored.
	 * @note: clears all previous setted data
	 */
	void copyDeepFiltered(IMidiEvents *_events, int filterChannel);
	//-------------------------------------------------------------------------
	void reserve(Int size) {
		events.reserve(size);
	}
	//-------------------------------------------------------------------------
	void insertFlat(const MidiEvent &ev) {
		events.push_back(ev);
	}
	//-------------------------------------------------------------------------
	void insertDeep(const MidiEvent &ev);

}; // DefaultMidiEvents
}} // namespace(s)

#endif /* SAMBAG_DEFAULTMIDIEVENTS_H */
