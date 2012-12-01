/*
 * IMidiEvents.hpp
 *
 *  Created on: Fri Nov 30 11:12:19 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IMIDIEVENTS_H
#define SAMBAG_IMIDIEVENTS_H

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class IMidiEvents.
  * Common interface for different midi event types on different plattforms.
  * Motivation: have an independent interface with an minimal effort on
  * data convertions.
  * Example (Vst):
  *  specific generation:
  *       | VstMidiEventAdapter independent(vstEventsObject);
  *       | // call event handler with independent object
  *       | handleEvent(independent);
  *  independent use:
  *       | IMidiEvents::MidiEvent event = independent->getMidiEvent(0);
  *       | ...
  *  specific unpacking:
  *	      | VstMidiEventAdapter flatcopy(independet); // no convertion
  *		  | XXXMidiEventAdapter deepcopy(independet); // convertion
  */
struct IMidiEvents {
//=============================================================================
	//-------------------------------------------------------------------------
	typedef int Int;
	//-------------------------------------------------------------------------
	typedef Int ByteSize;
	//-------------------------------------------------------------------------
	typedef int DeltaFrames;
	//-------------------------------------------------------------------------
	typedef char Data;
	//-------------------------------------------------------------------------
	typedef Data* DataPtr;
	//-------------------------------------------------------------------------
	typedef boost::tuple<ByteSize, DeltaFrames, DataPtr> MidiEvent;
	//-------------------------------------------------------------------------
	virtual Int getNumEvents() const = 0;
	//-------------------------------------------------------------------------
	virtual MidiEvent getMidiEvent(Int index) const = 0;
}; // IMidiEvents
///////////////////////////////////////////////////////////////////////////////
inline bool operator==(const IMidiEvents &a, const IMidiEvents &b) {
	if (a.getNumEvents() != b.getNumEvents() ) {
		return false;
	}
	int num = a.getNumEvents();
	for (int i=0; i<num; ++i) {
		IMidiEvents::MidiEvent eva = a.getMidiEvent(i);
		IMidiEvents::MidiEvent evb = b.getMidiEvent(i);
		if   ( boost::get<0>(eva) != boost::get<0>(evb) ||
			   boost::get<1>(eva) != boost::get<1>(evb)
			 )
		{
			return false;
		}
		IMidiEvents::ByteSize bytes = boost::get<0>(eva);
		for (int j=0; j<bytes; ++j) {
			if (boost::get<2>(eva)[j] != boost::get<2>(evb)[j]) 
				return false;
		}
	}
	return true;
}
inline bool operator!=(const IMidiEvents &a, const IMidiEvents &b) {
	return !(a==b);
}
}} // namespace(s)

#endif /* SAMBAG_IMIDIEVENTS_H */
