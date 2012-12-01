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
	DefaultMidiEvents a;
	for (int i=0; i<1024; ++i) {
		int bytes = i + 1;
		data[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data[i][j] = j % 26 + 'a';
		}
		a.events.push_back(IMidiEvents::MidiEvent(bytes, i, data[i].get()));
	}
	VstMidiEventAdapter b(&a); // deep copy
	VstMidiEventAdapter c(&b); // flat copy
	CPPUNIT_ASSERT(a==b);
	CPPUNIT_ASSERT(c==c);
	b.events->events[0]->data[0] = '!';
	CPPUNIT_ASSERT(a!=b);
	CPPUNIT_ASSERT(b==c);

	VstEvents * vstev = createVstEvents();
	VstMidiEventAdapter d(vstev);
	b.set(&d);
	CPPUNIT_ASSERT(b==d);
	d.set(&a);
	CPPUNIT_ASSERT(b!=d);

	DataArray data2[2024];
	DefaultMidiEvents e;
	for (int i=0; i<2024; ++i) {
		int bytes = 2024 - i;
		data2[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data2[i][j] = j % 26 + 'A';
		}
		e.events.push_back(IMidiEvents::MidiEvent(bytes, i, data2[i].get()));
	}
	d.set(&e); // assign larger data
	CPPUNIT_ASSERT(d==e);
	d.set(&a); // assign smaller data
	CPPUNIT_ASSERT(d==a);
}
} //namespace
