/*
 * Vst3MidiEvents.cpp
 *
 *  Created on: Mon Feb 16 21:10:23 2015
 *      Author: Johannes Unger
 */

#include "Vst3MidiEvents.hpp"
#include <sambag/com/Common.hpp>

namespace sambag { namespace dsp {
//=============================================================================
//  Class Vst3MidiEvents
//=============================================================================
//-----------------------------------------------------------------------------
Vst3MidiAdapter::Ptr Vst3MidiAdapter::create() {
    Ptr neu (new Vst3MidiAdapter());
    return neu;
}
//-----------------------------------------------------------------------------
void Vst3MidiAdapter::set(IMidiEvents::Ptr adaptee) {
    this->adaptee = trim(adaptee);
}
//-----------------------------------------------------------------------------
Steinberg::int32 Vst3MidiAdapter::getEventCount() {
    if (!adaptee) {
        return 0;
    }
    return (Steinberg::int32)adaptee->getNumEvents();
}
//-----------------------------------------------------------------------------
Steinberg::tresult
Vst3MidiAdapter::getEvent(Steinberg::int32 index, Steinberg::Vst::Event &e)
{
    using namespace Steinberg;
    if (!adaptee) {
        return Steinberg::kResultFalse;
    }
    IMidiEvents::MidiEvent midiEv = adaptee->getMidiEvent(index);
    e.sampleOffset = boost::get<1>(midiEv);
    e.busIndex = 0;
    switch (getEventType(midiEv)) {
        case IMidiEvents::NoteOn:
            if (getVelocity(midiEv)==0)
            {
                goto NOTEOFF;
            }
            e.type            = Vst::Event::kNoteOnEvent;
            e.noteOn.channel  = getChannel(midiEv);
            e.noteOn.pitch    = getPitch(midiEv);
            e.noteOn.velocity = getVelocity(midiEv);
            e.noteOn.length   = 0;
            e.noteOn.tuning   = 0;
            e.noteOn.noteId   = -1;
            return Steinberg::kResultOk;
        case IMidiEvents::NoteOff:
            NOTEOFF:
            e.type = Vst::Event::kNoteOffEvent;
            e.noteOff.channel  = getChannel(midiEv);
            e.noteOff.pitch    = getPitch(midiEv);
            e.noteOff.velocity = getVelocity(midiEv);
            e.noteOff.tuning   = 0;
            e.noteOff.noteId   = -1;
            return Steinberg::kResultOk;
        case IMidiEvents::Sysex:
            e.type       = Vst::Event::kDataEvent;
            e.data.bytes = boost::get<2>(midiEv);
            e.data.size  = boost::get<0>(midiEv);
            e.data.type  = Vst::DataEvent::kMidiSysEx;
            return Steinberg::kResultOk;
        case IMidiEvents::Aftertouch:
            e.type                  = Steinberg::Vst::Event::kPolyPressureEvent;
            e.polyPressure.channel  = getChannel(midiEv);
            e.polyPressure.pitch    = getPitch(midiEv);
            e.polyPressure.pressure = getPressure(midiEv);
            return Steinberg::kResultOk;
    }
    return Steinberg::kResultFalse;
}
//-----------------------------------------------------------------------------
Steinberg::tresult Vst3MidiAdapter::addEvent(Steinberg::Vst::Event &e) {
    SAMBAG_LOG_ERR<<"Vst3MidiEvents::addEvent not supported";
    return Steinberg::kResultFalse;
}
//-----------------------------------------------------------------------------
Steinberg::tresult Vst3MidiAdapter::queryInterface(const Steinberg::TUID, void** obj)
{
    *obj = NULL;
    return Steinberg::kNotImplemented;
}
}} // namespace(s)
