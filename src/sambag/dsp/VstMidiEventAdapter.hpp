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
	virtual MidiEvent getMidiEvent(Int index) const {
		//boost::tuple<ByteSize, DeltaFrames, DataPtr> 
		MidiEvent res;
		VstEvent *ev = events->events[index];
		if (ev->type != kVstMidiType) {
			boost::get<0>(res) = 0;
			return res;
		}
		boost::get<0>(res) = ev->byteSize;
		boost::get<1>(res) = ev->deltaFrames;
		boost::get<2>(res) = (DataPtr) &(ev->data);
		return res;
	}
	//-------------------------------------------------------------------------
	void set(IMidiEvents::Ptr ev);
	//-------------------------------------------------------------------------
	virtual ~VstMidiEventAdapter();

}; // VstMidiEventAdapter
}} // namespace(s)

#endif /* SAMBAG_VSTMIDIEVENTADAPTER_H */
