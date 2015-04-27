/*
 * TestVstMidiEventAdapter.cpp
 *
 *  Created on: Fri Nov 30 19:08:19 2012
 *      Author: Johannes Unger
 */

#include "TestVstMidiEventAdapter.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/dsp/DefaultMidiEvents.hpp>
#include <sambag/dsp/VstMidiEventAdapter.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestVstMidiEventAdapter );


enum { MAX_TMPEV = 2 };
VstEvent tmpEvent[MAX_TMPEV] = {0};
VstEvents tmpEv;

VstEvents * createVstEvents() {
	VstEvents * res = &tmpEv;
	res->numEvents = MAX_TMPEV;
	for (int i=0; i<res->numEvents; ++i) {
		res->events[i] = &tmpEvent[i];
		res->events[i]->type = kVstMidiType;
		res->events[i]->byteSize = 16;
		res->events[i]->deltaFrames = i;
		res->events[i]->flags = 0;
		for (int j=0; j<res->events[i]->byteSize; ++j) {
			res->events[i]->data[j] = j % 26 + 'a';
		}
	}
	return res;
}

namespace tests {
//=============================================================================
//  Class TestVstMidiEventAdapter
//=============================================================================
//-----------------------------------------------------------------------------
void TestVstMidiEventAdapter::testCreateNCopy() {
	using namespace sambag::dsp;
	typedef boost::shared_array<IMidiEvents::Data> DataArray;
	DataArray data[1024];
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	for (int i=0; i<1024; ++i) {
		int bytes = i + 1;
		data[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data[i][j] = j % 26 + 'a';
		}
		a->events.push_back(IMidiEvents::MidiEvent(bytes, i, data[i].get()));
	}
	VstMidiEventAdapter::Ptr b = VstMidiEventAdapter::create(a); // deep copy
	VstMidiEventAdapter::Ptr c = VstMidiEventAdapter::create(b); // flat copy
	CPPUNIT_ASSERT(*a==*b);
	CPPUNIT_ASSERT(*c==*c);
	b->events->events[0]->data[0] = '!';
	CPPUNIT_ASSERT(*a!=*b);
	CPPUNIT_ASSERT(*b==*c);

	VstEvents * vstev = createVstEvents();
	VstMidiEventAdapter::Ptr d = VstMidiEventAdapter::create(vstev);
	b->set(d);
	CPPUNIT_ASSERT(*b==*d);
	d->set(a);
	CPPUNIT_ASSERT(*b!=*d);

	DataArray data2[2024];
	DefaultMidiEvents::Ptr e = DefaultMidiEvents::create();
	for (int i=0; i<2024; ++i) {
		int bytes = 2024 - i;
		data2[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data2[i][j] = j % 26 + 'A';
		}
		e->events.push_back(IMidiEvents::MidiEvent(bytes, i, data2[i].get()));
	}
	d->set(e); // assign larger data
	CPPUNIT_ASSERT(*d==*e);
	d->set(a); // assign smaller data
	CPPUNIT_ASSERT(*d==*a);
}
//-----------------------------------------------------------------------------
namespace {
    void testEventData(sambag::dsp::IMidiEvents::Ptr ev, int nb, int delta, int bytessize,
        unsigned char d1, unsigned char d2, unsigned char d3 = 0,
        unsigned char d4 = 0, unsigned char d5 = 0, unsigned char d6 = 0,
        unsigned char d7 = 0)
    {
        using namespace sambag::dsp;
        IMidiEvents::DataPtr data = NULL;
        int deltaFrame = 0;
        int bytes = 0;
        CPPUNIT_ASSERT(ev->getNumEvents()>nb);
        boost::tie(bytes, deltaFrame, data) = ev->getMidiEvent(nb);
        CPPUNIT_ASSERT_EQUAL(bytessize, bytes);
        CPPUNIT_ASSERT(data!=NULL);
        CPPUNIT_ASSERT_EQUAL(data[0], d1);
        CPPUNIT_ASSERT_EQUAL(data[1], d2);
        if (d3!=0) {
            CPPUNIT_ASSERT_EQUAL(data[2], d3);
        }
        if (d4!=0) {
            CPPUNIT_ASSERT_EQUAL(data[3], d4);
        }
        if (d5!=0) {
            CPPUNIT_ASSERT_EQUAL(data[4], d5);
        }
        if (d6!=0) {
            CPPUNIT_ASSERT_EQUAL(data[5], d6);
        }
        if (d7!=0) {
            CPPUNIT_ASSERT_EQUAL(data[6], d7);
        }
    }
}
void TestVstMidiEventAdapter::testTrim() {
    /*
     * tests an issue which appears with reaper, two midi events in one event object.
     * ( a note off without a related note on)
     * according to other instruments (eg. http://thepiz.org/plugins/?p=midiMonitor) we handle only the first event
     * and ignore the rest.
     */
	using namespace sambag::dsp;
    DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
    // create data stream splitted to several events
    {
        IMidiEvents::Data data[] = {0,0,0,0,0,0,0,0,0x80,0x39,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x80,0x37,0,0,0,0,0,0};
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 0, &data[0]));
    }
    {
        IMidiEvents::Data data[] = {0,0,0,0,0,0,0,0,0x90,0x2b,0x69,0,0,0,0,0,0,0,0,0,0,0,0,0,0x80,0x37,0,0,0,0,0,0};
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 1, &data[0]));
    }
    {
        IMidiEvents::Data data[] = {0,0,0,0,0,0,0,0,0x80,0x2b,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x80,0x37,0,0,0,0,0,0};
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 2, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0xF0, 0x10, 0x20, 0x30, 0x40, 0xF7 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 6, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0xF0, 0x40, 0x50 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 6, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x80, 0x10, 0x0, 0xF7 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 60, &data[0]));
    }
    {
        IMidiEvents::Data data[] = {0,0,0,0,0,0,0,0,0x80,0x2b,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x80,0x37,0,0,0,0,0,0};
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 2, &data[0]));
    }
    IMidiEvents::Ptr b = VstMidiEventAdapter::create(a)->trim();
    CPPUNIT_ASSERT_EQUAL((int)6, b->getNumEvents());
    testEventData(b, 0, 1, 3, 0x80,0x39,0);
    testEventData(b, 1, 1, 3, 0x90,0x2b,0x69);
    testEventData(b, 2, 2, 3, 0x80,0x2b,0);
    testEventData(b, 3, 6, 6, 0xF0, 0x10, 0x20, 0x30, 0x40, 0xF7);
    testEventData(b, 4, 60, 7, 0xF0, 0x40, 0x50, 0x80, 0x10, 0x0, 0xF7);
    testEventData(b, 2, 2, 3, 0x80,0x2b,0);
}
} //namespace
