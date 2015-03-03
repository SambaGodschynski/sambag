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
class DefaultMidiEvents : public IMidiEvents {
//=============================================================================
protected:
    DefaultMidiEvents() {}
    DefaultMidiEvents(const DefaultMidiEvents&) {}
    DefaultMidiEvents & operator=(const DefaultMidiEvents&) {}
public:
    typedef boost::shared_ptr<DefaultMidiEvents> Ptr;
    typedef boost::weak_ptr<DefaultMidiEvents> WPtr;
	std::vector<MidiEvent> events;
	typedef boost::shared_array<Data> DataArray;
	std::vector<DataArray> dataContainer; // needed for deep copy
	//-------------------------------------------------------------------------
	static Ptr create(IMidiEvents::Ptr _events = IMidiEvents::Ptr()) {
        Ptr neu( new DefaultMidiEvents() );
		if (_events) {
			neu->copyFlat(_events);
		}
        return neu;
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
	void copyFlat(IMidiEvents::Ptr _events);
	//-------------------------------------------------------------------------
	/**
     * @brief deep copy of midi data.
	 * @note: clears all previous setted data
	 */
	void copyDeep(IMidiEvents::Ptr _events);
	//-------------------------------------------------------------------------
	/**
    * @brief deep copy of midi data using a channel filter.
	 * all channel related messages wich not belong to filterChannel will be
	 * ignored.
	 * @note: clears all previous setted data
	 */
	void copyDeepFiltered(IMidiEvents::Ptr _events, int filterChannel);
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
