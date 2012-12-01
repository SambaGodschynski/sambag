/*
 * TestDefaultMidiEvents.cpp
 *
 *  Created on: Fri Nov 30 14:19:00 2012
 *      Author: Johannes Unger
 */

#include "TestDefaultMidiEvents.hpp"
#include <sambag/dsp/DefaultMidiEvents.hpp>
#include <cppunit/config/SourcePrefix.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestDefaultMidiEvents );

namespace tests {
//=============================================================================
//  Class TestDefaultMidiEvents
//=============================================================================
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopy() {
	using namespace sambag::dsp;
	typedef boost::shared_array<IMidiEvents::Data> DataArray;
	DataArray data[1024];
	DefaultMidiEvents a;
	for (int i=0; i<1024; ++i) {
		int bytes = (i%15) + 1;
		data[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data[i][j] = j % 26 + 'a';
		}
		a.events.push_back(IMidiEvents::MidiEvent(bytes, i, data[i].get()));
	}
	DefaultMidiEvents b(&a); // flat copy
	CPPUNIT_ASSERT(a==b);
	DefaultMidiEvents c;
	c.copyDeep(&a);
	CPPUNIT_ASSERT(c==a);
	data[0][0] = '!';
	CPPUNIT_ASSERT(c!=a);
	CPPUNIT_ASSERT(c!=b);
	CPPUNIT_ASSERT(a==b);
}
} //namespace
