/*
 * IMidiEvents.cpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#include "IMidiEvents.hpp"
#include "DefaultMidiEvents.hpp"
#include <boost/foreach.hpp>
#include <vector>

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
//-----------------------------------------------------------------------------
namespace {
    void copySysex(MidiDataIterator &it, std::vector<IMidiEvents::Data> &dst)
    {
        while(true) {
            IMidiEvents::Data date = 0;
            if (!it.nextByte(&date)) {
                SAMBAG_THROW(MidiDataError, "IMidiEvents::copySysex incomplete sysex");
            }
            dst.push_back(date);
            if (date==0xF7) {
                return;
            }
        }
    }
}
IMidiEvents::Ptr trim(IMidiEvents::Ptr ev) {
    if (!ev) {
        return ev;
    }
    typedef IMidiEvents::MidiEvent MidiEvent;
    MidiDataIterator it(ev);
    DefaultMidiEvents::Ptr newEv = DefaultMidiEvents::create();
    while(true) {
        IMidiEvents::Data date = 0;
        if (!it.nextByte(&date)) {
            return newEv;
        }
        if (date==0xF2) {
            it.nextByte(&date);
            it.nextByte(&date);
            continue;
        }
        if (date==0xF3) {
            it.nextByte(&date);
            continue;
        }
        if (date>=0xF1) {
            // 0xF1, 0xF4 .. 0xFF -> one byte events
            continue;
        }
        switch (date>>4) {
            throw std::logic_error("MIDI MESSAGES NOT COMPLETE");
            case 0x8: // 3 byte events
            case 0x9:
            case 0xA:
            case 0xB:
            case 0xE:
            {
                int delta = it.deltaFrames();
                IMidiEvents::Data data[3] = {date, 0, 0};
                if ( !it.nextByte(&data[1])
                    || !it.nextByte(&data[2]))
                {
                    // incomplete event
                    return newEv;
                }
                newEv->insertDeep(MidiEvent(3, delta, &data[0]));
                continue;
            }
            case 0xC:
            case 0xD: // 2 byte events
            {
                int delta = it.deltaFrames();
                IMidiEvents::Data data[2] = {date, 0};
                if (!it.nextByte(&data[1]))
                {
                    // incomplete event
                    throw std::runtime_error("IMidiEvents::trim incomplete midi data");
                }
                newEv->insertDeep(MidiEvent(2, delta, &data[0]));
                continue;
            }
            case 0xF: // sysex
            {
                int delta = it.deltaFrames();
                std::vector<IMidiEvents::Data> tmp;
                tmp.push_back(date);
                copySysex(it, tmp);
                IMidiEvents::Data *tmp2 = new IMidiEvents::Data[tmp.size()];
                int c=0;
                BOOST_FOREACH(IMidiEvents::Data x, tmp) {
                    tmp2[c++] = x;
                }
                newEv->insertDeep(MidiEvent(tmp.size(), delta, tmp2));
                delete tmp2;
                continue;
            }
            default: continue;
        }
    }
}
}}