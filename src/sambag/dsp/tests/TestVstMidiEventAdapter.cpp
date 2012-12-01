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
}
} //namespace
