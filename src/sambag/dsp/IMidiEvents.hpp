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
  *	      | MidiEventConverter::getVstEvents(independet); // no convertion
  *		  | MidiEventConverter::getXXXEvents(independet); // convertion
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
}} // namespace(s)

#endif /* SAMBAG_IMIDIEVENTS_H */
