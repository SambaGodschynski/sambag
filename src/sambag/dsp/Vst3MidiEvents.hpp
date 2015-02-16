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

namespace sambag { namespace dsp {

//=============================================================================
/** 
  * @class Vst3MidiEvents.
  * @brief a Vst3 Event that adatps a sambag::IMidiEvent
  */
class Vst3Adapter : Steinberg::Vst::IEventList {
//=============================================================================
public:
    typedef boost::shared_ptr<Vst3Adapter> Ptr;
    typedef boost::weak_ptr<Vst3Adapter> WPtr;

protected:
    Vst3Adapter() {}
    Vst3Adapter(const Vst3Adapter&) {}
    Vst3Adapter & operator=(const Vst3Adapter&) {return *this;}
private:
    IMidiEvents::Ptr adaptee;
public:
    static Ptr create();
    void set(IMidiEvents::Ptr adaptee);
    ///////////////////////////////////////////////////////////////////////////
    Steinberg::tresult queryInterface (const Steinberg::TUID, void** obj);
    Steinberg::uint32 addRef() { return 1; }
    Steinberg::uint32 release() { return 1; }
    virtual Steinberg::int32 getEventCount ();
    virtual Steinberg::tresult getEvent (Steinberg::int32 index, Steinberg::Vst::Event &e);
    virtual Steinberg::tresult addEvent (Steinberg::Vst::Event &e);
}; // Vst3Adapter


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
