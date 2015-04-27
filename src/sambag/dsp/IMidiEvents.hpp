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
#include <boost/enable_shared_from_this.hpp>

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
struct IMidiEvents : public boost::enable_shared_from_this<IMidiEvents> {
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
    //-------------------------------------------------------------------------
    /**
     * @brief removes unused midi bytes. implemented by VstMidiEventAdapter
     * an issue which appears with reaper: two midi events in one event object.
     * ( a note off without a related note on)
     * according to other instruments (eg. http://thepiz.org/plugins/?p=midiMonitor) 
     * we handle only the first event and ignore the rest.
     * @note when nothing was changed the returned events == the given events
     */
    virtual IMidiEvents::Ptr trim() { return shared_from_this(); }
}; // IMidiEvents
///////////////////////////////////////////////////////////////////////////////
extern bool operator==(const IMidiEvents &a, const IMidiEvents &b);
inline bool operator!=(const IMidiEvents &a, const IMidiEvents &b)
{
	return !(a==b);
}
extern std::string toString(const IMidiEvents &ev);
inline std::ostream & operator << (std::ostream& os, const IMidiEvents &b)
{
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
    /**
     * @return the current event index
     */
    int getIdxEvent() { return currEventIdx-1; /* idx points to the next ev */ }
    /**
     * @return the current byte index
     */
    int getIdxByte() { return currByte; }
    /**
     * @brief seeks to a position in the IMidiEvents object.
     * @throws if position invalid
     */
    bool seek(int idxEv, int idxByte);
    
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
