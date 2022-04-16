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
    if (!adaptee) {
        _events.clear();
        return;
    }
    // we have to filter valid midi events
    IMidiEvents::Ptr ev = adaptee->trim();
    int num = ev->getNumEvents();
    _events.reserve(num);
    for (int i=0; i<num; ++i) {
        Steinberg::Vst::Event e;
        if (!getEvent(ev, i, e)) {
            continue;
        }
        _events.push_back(e);
    }
}
//-----------------------------------------------------------------------------
Steinberg::int32 PLUGIN_API
Vst3MidiAdapter::getEventCount() {
    return _events.size();
}
//-----------------------------------------------------------------------------
bool Vst3MidiAdapter::
getEvent(IMidiEvents::Ptr adaptee, Steinberg::int32 index, Steinberg::Vst::Event &e)
{
    using namespace Steinberg;
    if (!adaptee) {
        return false;
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
            e.noteOn.velocity = getVelocity(midiEv) / 127.0f;
            e.noteOn.length   = 0;
            e.noteOn.tuning   = 0;
            e.noteOn.noteId   = -1;
            return true;
        case IMidiEvents::NoteOff:
            NOTEOFF:
            e.type = Vst::Event::kNoteOffEvent;
            e.noteOff.channel  = getChannel(midiEv);
            e.noteOff.pitch    = getPitch(midiEv);
            e.noteOff.velocity = getVelocity(midiEv) / 127.0f;
            e.noteOff.tuning   = 0;
            e.noteOff.noteId   = -1;
            return true;
        case IMidiEvents::Sysex:
            e.type       = Vst::Event::kDataEvent;
            e.data.bytes = boost::get<2>(midiEv);
            e.data.size  = boost::get<0>(midiEv);
            e.data.type  = Vst::DataEvent::kMidiSysEx;
            return true;
        case IMidiEvents::Aftertouch:
            e.type                  = Steinberg::Vst::Event::kPolyPressureEvent;
            e.polyPressure.channel  = getChannel(midiEv);
            e.polyPressure.pitch    = getPitch(midiEv);
            e.polyPressure.pressure = getPressure(midiEv) / 127.0f;
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API
Vst3MidiAdapter::getEvent(Steinberg::int32 index, Steinberg::Vst::Event &e)
{
    if (index < 0 || index > _events.size()) {
        return Steinberg::kResultFalse;
    }
    e = _events[index];
    return Steinberg::kResultOk;
}
//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API
Vst3MidiAdapter::addEvent(Steinberg::Vst::Event &e) {
    SAMBAG_LOG_ERR<<"Vst3MidiEvents::addEvent not supported";
    return Steinberg::kResultFalse;
}
//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API
Vst3MidiAdapter::queryInterface(const Steinberg::TUID, void** obj)
{
    *obj = NULL;
    return Steinberg::kNotImplemented;
}
}} // namespace(s)
