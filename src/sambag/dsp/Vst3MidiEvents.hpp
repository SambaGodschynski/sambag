/*
 * Vst3MidiEvents.hpp
 *
 *  Created on: Mon Feb 16 21:10:23 2015
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_VST3MIDIEVENTS_H
#define SAMBAG_VST3MIDIEVENTS_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "IMidiEvents.hpp"
#include <ivstevents.h>
#include <vector>

namespace sambag { namespace dsp {

//=============================================================================
/** 
  * @class Vst3MidiEvents.
  * @brief a Vst3 Event that adatps a sambag::IMidiEvent
  */
class Vst3MidiAdapter : public Steinberg::Vst::IEventList {
//=============================================================================
public:
    typedef boost::shared_ptr<Vst3MidiAdapter> Ptr;
    typedef boost::weak_ptr<Vst3MidiAdapter> WPtr;

protected:
    Vst3MidiAdapter() {}
    Vst3MidiAdapter(const Vst3MidiAdapter&) {}
    Vst3MidiAdapter & operator=(const Vst3MidiAdapter&) {return *this;}
    virtual bool getEvent (IMidiEvents::Ptr adaptee, Steinberg::int32 index, Steinberg::Vst::Event &e);
private:
    std::vector<Steinberg::Vst::Event> _events;
public:
    static Ptr create();
    void set(IMidiEvents::Ptr adaptee);
    ///////////////////////////////////////////////////////////////////////////
    Steinberg::tresult PLUGIN_API queryInterface (const Steinberg::TUID, void** obj);
    Steinberg::uint32 PLUGIN_API addRef() { return 1; }
    Steinberg::uint32 PLUGIN_API release() { return 1; }
    virtual Steinberg::int32 PLUGIN_API getEventCount ();
    virtual Steinberg::tresult PLUGIN_API getEvent (Steinberg::int32 index, Steinberg::Vst::Event &e);
    virtual Steinberg::tresult PLUGIN_API addEvent (Steinberg::Vst::Event &e);
}; // Vst3MidiAdapter


//=============================================================================
/** 
  * @class Vst3MidiEvents.
  * @brief a sambag::IMidiEvent that adaps a Vst3 Event
  */
class IMidiAdapter : public IMidiEvents {
//=============================================================================
public:
    typedef boost::shared_ptr<IMidiAdapter> Ptr;
    typedef boost::weak_ptr<IMidiAdapter> WPtr;
protected:
    IMidiAdapter() {}
    IMidiAdapter(const IMidiAdapter&) {}
    IMidiAdapter & operator=(const IMidiAdapter&) {return *this;}
private:
public:
}; // Vst3MidiEvents


}} // namespace(s)

#endif /* SAMBAG_VST3MIDIEVENTS_H */
