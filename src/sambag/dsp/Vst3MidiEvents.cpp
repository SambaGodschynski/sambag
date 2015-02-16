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
Vst3Adapter::Ptr Vst3Adapter::create() {
    Ptr neu (new Vst3Adapter());
    return neu;
}
//-----------------------------------------------------------------------------
void Vst3Adapter::set(IMidiEvents::Ptr adaptee) {
    this->adaptee = adaptee;
}
//-----------------------------------------------------------------------------
Steinberg::int32 Vst3Adapter::getEventCount() {
    if (!adaptee) {
        return 0;
    }
    return (Steinberg::int32)adaptee->getNumEvents();
}
//-----------------------------------------------------------------------------
Steinberg::tresult
Vst3Adapter::getEvent(Steinberg::int32 index, Steinberg::Vst::Event &e)
{
    if (!adaptee) {
        return Steinberg::kResultFalse;
    }
    
}
//-----------------------------------------------------------------------------
Steinberg::tresult Vst3Adapter::addEvent(Steinberg::Vst::Event &e) {
    SAMBAG_LOG_ERR<<"Vst3MidiEvents::addEvent not supported";
    return Steinberg::kResultFalse;
}
//-----------------------------------------------------------------------------
Steinberg::tresult Vst3Adapter::queryInterface(const Steinberg::TUID, void** obj)
{
    *obj = nullptr;
    return Steinberg::kNotImplemented;
}
}} // namespace(s)
