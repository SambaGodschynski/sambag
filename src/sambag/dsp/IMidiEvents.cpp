/*
 * IMidiEvents.cpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#include "IMidiEvents.hpp"
namespace sambag { namespace dsp {
namespace {
    typedef IMidiEvents::ByteSize ByteSize;
    typedef IMidiEvents::DeltaFrames DeltaFrames;
    typedef IMidiEvents::Data Data;
    typedef IMidiEvents::DataPtr DataPtr;
}
//-----------------------------------------------------------------------------
IMidiEvents::EventType getEventType(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    DataPtr data = boost::get<2>(ev);
    if(size<3) {
        return IMidiEvents::Unknown;
    }
    switch (data[0]>>4) {
        case 0x8: return IMidiEvents::NoteOff;
        case 0x9: return IMidiEvents::NoteOn;
        case 0xA: return IMidiEvents::Aftertouch;
        case 0xB: return IMidiEvents::Cc;
        case 0xC: return IMidiEvents::Pc;
        case 0xD: return IMidiEvents::Aftertouch;
        case 0xE: return IMidiEvents::PitchBend;
        case 0xF: return IMidiEvents::Sysex;
        default: return IMidiEvents::Unknown;
    }
}
//-----------------------------------------------------------------------------
int getChannel(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<1) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[0] & 0x0F;
}
//-----------------------------------------------------------------------------
int getPitch(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<2) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[1];
}
//-----------------------------------------------------------------------------
int getVelocity(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<3) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[2];
}
//-----------------------------------------------------------------------------
int getCc(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<3) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[1];
}
//-----------------------------------------------------------------------------
int getCcValue(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<3) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[2];
}
//-----------------------------------------------------------------------------
int getPc(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<2) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[1];
}
//-----------------------------------------------------------------------------
int getPressure(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<2) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return data[1];
}
//-----------------------------------------------------------------------------
int getPitchBend(const IMidiEvents::MidiEvent &ev)
{
    ByteSize size = boost::get<0>(ev);
    if(size<3) {
        throw std::runtime_error("invalid midi data");
    }
    DataPtr data = boost::get<2>(ev);
    return (data[2]<<7) + data[1];
}
///////////////////////////////////////////////////////////////////////////////
MidiDataIterator::MidiDataIterator(IMidiEvents::Ptr src) :
    currEvent(NULL)
  , currEventIdx(0)
  , bytes(0)
  , currByte(0)
  , src(src)
{
}
//-----------------------------------------------------------------------------
bool MidiDataIterator::nextEvent() {
    if ( currEventIdx >= src->getNumEvents() ) {
        return false;
    }
    currEvent = src->getMidiEvent(currEventIdx++);
    bytes = boost::get<0>(currEvent);
    currByte = 0;
    return true;
}
//-----------------------------------------------------------------------------
int MidiDataIterator::deltaFrames() const {
    return boost::get<1>(currEvent);
}
//-----------------------------------------------------------------------------
bool MidiDataIterator::nextByte(IMidiEvents::Data *outByte) {
    if (currByte >= bytes) {
        if (!nextEvent()) {
            return false;
        }
    }
    *outByte = boost::get<2>(currEvent)[currByte++];
    return true;
}
}}