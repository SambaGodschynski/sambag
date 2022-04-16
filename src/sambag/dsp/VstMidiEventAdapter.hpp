/*
 * MidiEventAdapter.hpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_VSTMIDIEVENTADAPTER_H
#define SAMBAG_VSTMIDIEVENTADAPTER_H

#include <boost/shared_ptr.hpp>
#include "IMidiEvents.hpp"
#include <aeffectx.h>
#include <sambag/com/Exception.hpp>
#include <ostream>

namespace sambag { namespace dsp {
//=============================================================================
/** 
  * @class VstMidiEventAdapter.
  */
class VstMidiEventAdapter : public IMidiEvents {
//=============================================================================
protected:
    VstMidiEventAdapter() {}
    VstMidiEventAdapter(const VstMidiEventAdapter&) {}
    VstMidiEventAdapter & operator=(const VstMidiEventAdapter&) {}
public:
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<VstMidiEventAdapter> Ptr;
    typedef boost::weak_ptr<VstMidiEventAdapter> WPtr;
	VstEvents *events;
	Int maxEvents;
	bool ownerOfData;
	//-------------------------------------------------------------------------
	static Ptr create(VstEvents *events)
	{
        Ptr neu(new VstMidiEventAdapter());
        neu->events=events;
        neu->ownerOfData = false;
        return neu;
	}
    //-------------------------------------------------------------------------
    static Ptr create(IMidiEvents::Ptr ev);
	//-------------------------------------------------------------------------
	static Ptr create(VstMidiEventAdapter::Ptr b)
    {
        Ptr neu(new VstMidiEventAdapter());
		neu->events = b->events;
		neu->ownerOfData = false;
        return neu;
	}
	//-------------------------------------------------------------------------
	void allocDataIfNecessary(IMidiEvents::Ptr ev);
	//-------------------------------------------------------------------------
	void freeDataIfNecessary();
	//-------------------------------------------------------------------------
	VstEvent * allocVstEventNecessary(size_t bytes, VstEvent *old);
	//-------------------------------------------------------------------------
	virtual Int getNumEvents() const {
		return events->numEvents;
	}
	//-------------------------------------------------------------------------
	virtual MidiEvent getMidiEvent(Int index) const;
    //-------------------------------------------------------------------------
	void set(IMidiEvents::Ptr ev);
	//-------------------------------------------------------------------------
	virtual ~VstMidiEventAdapter();
    //-------------------------------------------------------------------------
    /**
     * @brief removes unused midi bytes. implemented by VstMidiEventAdapter
     * @note when nothing was changed the returned events == the given events
     */
    virtual IMidiEvents::Ptr trim();
}; // VstMidiEventAdapter
}} // namespace(s)

///////////////////////////////////////////////////////////////////////////////
std::ostream & operator<<(std::ostream &os, const VstEvents &events);

#endif /* SAMBAG_VSTMIDIEVENTADAPTER_H */
