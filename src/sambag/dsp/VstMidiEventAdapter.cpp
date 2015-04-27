/*
 * VstMidiEventAdapter.cpp
 *
 *  Created on: Fri Nov 30 11:12:28 2012
 *      Author: Johannes Unger
 */

#include "VstMidiEventAdapter.hpp"
#include "DefaultMidiEvents.hpp"

namespace sambag { namespace dsp {

namespace {
	typedef IMidiEvents::Data Data;
	VstEvents * allocVstEvents(size_t numEvents) {
		size_t size = sizeof(VstEvents) + (numEvents * sizeof(VstEvent*));
		size += numEvents * sizeof(VstEvent);
		VstEvents *res = (VstEvents*)new Data[size];
		memset(res, 0, size);
		return res;
	}
	VstEvent * allocVstEvent(size_t bytes) {
		size_t size = sizeof(VstEvent) + (bytes * sizeof(Data*));
		size += (bytes) * sizeof(Data);
		VstEvent * res = (VstEvent*)new Data[size];
		return res;
	}
} // namespace 

//=============================================================================
// class VstMidiEventAdapter.
//=============================================================================
//-----------------------------------------------------------------------------
void VstMidiEventAdapter::allocDataIfNecessary(IMidiEvents::Ptr ev) {
	if (!ownerOfData || !events) {
		events = allocVstEvents(ev->getNumEvents());
		maxEvents = ev->getNumEvents();
		ownerOfData = true;
		return;
	}
	
	if (maxEvents < ev->getNumEvents()) {
		freeDataIfNecessary();
		events = allocVstEvents(ev->getNumEvents());
		maxEvents = ev->getNumEvents();
		return;
	}
}
//-----------------------------------------------------------------------------
VstEvent * VstMidiEventAdapter::allocVstEventNecessary(size_t bytes, VstEvent *old) 
{
	if (!old) {
		return allocVstEvent(bytes);
	}
	if (old->byteSize < (int)bytes) {
		delete old;
		return allocVstEvent(bytes);
	}
	return old;
}
//-----------------------------------------------------------------------------
void VstMidiEventAdapter::set(IMidiEvents::Ptr ev) {
	// special case:
	VstMidiEventAdapter::Ptr _ev =
		boost::dynamic_pointer_cast<VstMidiEventAdapter>(ev);
	if (_ev) {
		freeDataIfNecessary();
		this->events = _ev->events;
		ownerOfData = false;
		return;
	}
	allocDataIfNecessary(ev);
	events->numEvents = ev->getNumEvents();
	for (Int i=0; i<events->numEvents; ++i) {
		MidiEvent tmp = ev->getMidiEvent(i);
		events->events[i] = 
			allocVstEventNecessary(boost::get<0>(tmp), events->events[i]);
		events->events[i]->type = kVstMidiType;
		events->events[i]->byteSize = boost::get<0>(tmp);
		events->events[i]->deltaFrames = boost::get<1>(tmp);
		events->events[i]->flags = 0;
		DataPtr bytes = boost::get<2>(tmp);
		for (Int j=0; j<events->events[i]->byteSize; ++j) {
			events->events[i]->data[j] = bytes[j];
		}
		//memcpy(events->events[i]->data, bytes, events->events[i]->byteSize);
	}
}
//-----------------------------------------------------------------------------
VstMidiEventAdapter::MidiEvent VstMidiEventAdapter::getMidiEvent(Int index) const
{
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
//-----------------------------------------------------------------------------
void VstMidiEventAdapter::freeDataIfNecessary() {
	if (!ownerOfData) {
		return;
	}
	if (!events)
		return;
	for (Int i=0; i<maxEvents; ++i) {
		delete events->events[i];
	}
	delete events;
	events = NULL;
}
//-----------------------------------------------------------------------------
VstMidiEventAdapter::Ptr VstMidiEventAdapter::create(IMidiEvents::Ptr ev)
{
    Ptr neu(new VstMidiEventAdapter());
    neu->events = NULL;
    neu->ownerOfData = false;
	neu->set(ev);
    return neu;
}
//-----------------------------------------------------------------------------
VstMidiEventAdapter::~VstMidiEventAdapter() {
	freeDataIfNecessary();
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
//-----------------------------------------------------------------------------
IMidiEvents::Ptr VstMidiEventAdapter::trim() {
    typedef IMidiEvents::MidiEvent MidiEvent;
    DefaultMidiEvents::Ptr newEv = DefaultMidiEvents::create();
    if (!events) {
        return newEv;
    }
    for (int i=0; i<events->numEvents; ++i) {
        VstEvent *ev = events->events[i];
        if (!ev || ev->type != kVstMidiType) {
            continue;
        }
        int bytes = ev->byteSize;
        char * data = ev->data;
        for (int j=0; j<bytes; ++j)
        {
            unsigned char date = data[j];
            if (date>=0xF1) { // 0xF1, 0xF4 .. 0xFF
                break;
            }
            
            if (date==0xF0) // sysex
            {
                MidiDataIterator it(shared_from_this());
                it.seek(i,j);
                std::vector<IMidiEvents::Data> tmp;
                copySysex(it, tmp);
                IMidiEvents::Data *tmp2 = new IMidiEvents::Data[tmp.size()];
                int c=0;
                BOOST_FOREACH(IMidiEvents::Data x, tmp) {
                    tmp2[c++] = x;
                }
                newEv->insertDeep(MidiEvent(tmp.size(), ev->deltaFrames, tmp2));
                delete tmp2;
                i = it.getIdxEvent(); // jump to event where sysex ends
                break;
            }
            
            int status = date >> 4;
            
            if (status==0x8 || status==0x9 || status == 0xA || status == 0xB || status == 0xE)
            { // 3 byte events
                if (j+2>=bytes) {
                    // incomplete event
                    throw std::runtime_error("IMidiEvents::trim incomplete midi data");
                }
                IMidiEvents::Data copy[3] = {date, data[j+1], data[j+2]};
                newEv->insertDeep(MidiEvent(3, ev->deltaFrames, &copy[0]));
                break;
            }
            if (status==0xC || status==0xD) // 2 byte events
            {
                if (j+1>=bytes) {
                    // incomplete event
                    throw std::runtime_error("IMidiEvents::trim incomplete midi data");
                }
                IMidiEvents::Data copy[2] = {date, data[j+1]};
                newEv->insertDeep(MidiEvent(2, ev->deltaFrames, &copy[0]));
                break;
            }
        }
    }
    return  newEv;
}

}} // namespace(s)

///////////////////////////////////////////////////////////////////////////////
std::ostream & operator<<(std::ostream &os, const VstEvents &events)
{
    for (int i=0; i<events.numEvents; ++i) {
        const VstEvent * const ev = events.events[i];
        if (ev->type != kVstMidiType) {
            continue;
        }
        int size = ev->byteSize;
        const char * const data = ev->data;
        int delta = ev->deltaFrames;
        os<<"["<<delta<<"]: ";
        for (int j = 0; j<size; ++j) {
            os<<std::hex<<(int)data[j]<<" "<<std::dec;
        }
    }
    return os;
}
