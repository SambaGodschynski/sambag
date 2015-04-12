/*
 * IMidiEvents.hpp
 *
 *  Created on: Fri Nov 30 11:12:19 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IMIDIEVENTS_H
#define SAMBAG_IMIDIEVENTS_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/integer.hpp>
#include <sstream>
#include <ostream>
#include <vector>
#include <sambag/com/Exception.hpp>

namespace sambag { namespace dsp {
//-----------------------------------------------------------------------------
SAMBAG_EXCEPTION_CLASS(MidiDataError);
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
  *	      | VstMidiEventAdapter set(independet); // no convertion
  *		  | XXXMidiEventAdapter set(independet); // convertion
  */
struct IMidiEvents {
//=============================================================================
    //-------------------------------------------------------------------------
    enum EventType { Unknown,
                     NoteOn,
                     NoteOff,
                     Sysex,
                     Aftertouch,
                     Cc,
                     Pc,
                     PitchBend};
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<IMidiEvents> Ptr;
    typedef boost::weak_ptr<IMidiEvents> WPtr;
	//-------------------------------------------------------------------------
	typedef boost::int_t<32>::exact Int;
	//-------------------------------------------------------------------------
	typedef Int ByteSize;
	//-------------------------------------------------------------------------
	typedef boost::int_t<32>::exact DeltaFrames;
	//-------------------------------------------------------------------------
	typedef unsigned char Data;
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
inline std::string toString(const IMidiEvents &ev) {
	size_t numEv = ev.getNumEvents();
	std::stringstream ss;
	for ( size_t i=0; i<numEv; ++i ) {
		IMidiEvents::ByteSize size;
		IMidiEvents::DeltaFrames d;
		IMidiEvents::DataPtr data;
		boost::tie(size, d, data) = ev.getMidiEvent(i);
		for (int j=0; j<size; ++j) {
			unsigned int val = data[j];
			ss<<std::hex<<val<<" ";
		}
	}
	return ss.str();
}
inline std::ostream & operator << (std::ostream& os, const IMidiEvents &b) {
	os<<toString(b);
	return os;
}


///////////////////////////////////////////////////////////////////////////////
// HELPER
/**
 * @brief Iterates through the raw data of a midi event
 */
class MidiDataIterator {
    IMidiEvents::MidiEvent currEvent; // the current event
    int currEventIdx; // the current event
    int bytes; // the current events bytesize
    int currByte; // the current byte
    IMidiEvents::Ptr src;
    bool nextEvent();
public:
    MidiDataIterator(IMidiEvents::Ptr src);
    /**
     * @return the next midi data byte and true, or false if no further data.
     */
    bool nextByte(IMidiEvents::Data *outByte);
    /**
     * @return the delta frames of the current byte
     */
    int deltaFrames() const;
};

/**
 * @brief some DAW's (Repaer for instance) sends midi data with a lot of 
 * zeros in it. Event1: => 0x00 0x00 0x00 0x00 THE ACTUAL EVENT 0x00 0x00 
 * Since some algorithm assumes that every event has only data of one Midi event
 * we trim the thata in that way that every event is related to one midi event date.
 */
extern IMidiEvents::Ptr trim(IMidiEvents::Ptr ev);

/**
 * @return vector with bytestream representation of MidiEvents
 * @note we use a vector because it's storage has a usefull auto grow up strategy
 */
extern void createFlatRawData(const IMidiEvents &ev, std::vector<IMidiEvents::Data> &out);
/**
 * @brief unpacks datastream and creates MidiEvent
 * @note uses DefaultMidiEvents as concrete container.
 */
extern IMidiEvents::Ptr createMidiEvents(IMidiEvents::DataPtr data, size_t byteSize);

/**
 * @return the event type of the event
 */
extern IMidiEvents::EventType getEventType(const IMidiEvents::MidiEvent &ev);
/**
 * @return the channel type of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getChannel(const IMidiEvents::MidiEvent &ev);
/**
 * @return the pitch of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getPitch(const IMidiEvents::MidiEvent &ev);
/**
 * @return the velocity of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getVelocity(const IMidiEvents::MidiEvent &ev);
/**
 * @return the controller of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getCc(const IMidiEvents::MidiEvent &ev);
/**
 * @return the controller value of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getCcValue(const IMidiEvents::MidiEvent &ev);
/**
 * @return the program of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getPc(const IMidiEvents::MidiEvent &ev);
/**
 * @return the aftertouch pressure of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getPressure(const IMidiEvents::MidiEvent &ev);
/**
 * @return the pitchbend value of the event
 * @note does not check if result fits to the actual eventtype
 */
extern int getPitchBend(const IMidiEvents::MidiEvent &ev);


}} // namespace(s)

#endif /* SAMBAG_IMIDIEVENTS_H */
