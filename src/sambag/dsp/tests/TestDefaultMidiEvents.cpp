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
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0x90, 0x3C, 0x87, // NoteOnChannel(channel 0), c´, vel.120    
		0x80, 0x3C, 0x0,  // NoteOffChannel(channel 0), c´, vel.0
		0x91, 0x3C, 0x0,  // channel 1
		0x81, 0x3C, 0x0,
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents a;
	a.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents b;
	b.copyDeepFiltered(&a, 0);

	IMidiEvents::Data _assumeCh00[] = {
		0x90, 0x3C, 0x87, 
		0x80, 0x3C, 0x0,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents assumeCh00;
	assumeCh00.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(assumeCh00, b);

}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered02() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0x00, 0x00, 0x00, 
		0x90, 0x3C, 0x87, // NoteOnChannel(channel 0), c´, vel.120    
		0x80, 0x3C, 0x0,  // NoteOffChannel(channel 0), c´, vel.0
		0x91, 0x3C, 0x0,  // channel 1
		0x81, 0x3C, 0x0,
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents a;
	a.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents b;
	b.copyDeepFiltered(&a, 0);

	IMidiEvents::Data _assumeCh00[] = {
		0x00, 0x00, 0x00,
		0x90, 0x3C, 0x87, 
		0x80, 0x3C, 0x0,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents assumeCh00;
	assumeCh00.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(assumeCh00, b);

}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered03() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0x90, 0x3C, 0x87, // NoteOnChannel(channel 0), c´, vel.120    
		0x80, 0x80, 0x0,  // NoteOffChannel(channel 0), c´, vel.0
		0x91, 0x91, 0x0,  // channel 1
		0x81, 0x3C, 0x90, 0x81 // missing two bytes
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents a;
	a.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents b;
	b.copyDeepFiltered(&a, 1);

	IMidiEvents::Data _assumeCh00[] = {
		0x91, 0x91, 0x0, 
		0x81, 0x3C, 0x90,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents assumeCh00;
	assumeCh00.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(assumeCh00, b);
}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered04() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0x90, 0x3C, 0x87, // NoteOnChannel(channel 0), c´, vel.120    
		0x80, 0x80, 0x0,  // NoteOffChannel(channel 0), c´, vel.0
		0x91, 0x91, 0x0,  // channel 1
		0x81, 0x3C, 0x90, 0x81, 0x00 // missing one bytes
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents a;
	a.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents b;
	b.copyDeepFiltered(&a, 1);

	IMidiEvents::Data _assumeCh00[] = {
		0x91, 0x91, 0x0, 
		0x81, 0x3C, 0x90,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents assumeCh00;
	assumeCh00.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(assumeCh00, b);
}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered05() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0xf0, 0x00, 0x00,
		0x90, 0x00, 0x00, 0xf7, // sysex
		0x90, 0x3C, 0x87, // NoteOnChannel(channel 0), c´, vel.120    
		0x80, 0x80, 0x0,  // NoteOffChannel(channel 0), c´, vel.0
		0x91, 0x91, 0x0,  // channel 1
		0x81, 0x3C, 0x90, 0x81, 0x00 // missing one bytes
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents a;
	a.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents b;
	b.copyDeepFiltered(&a, 1);

	IMidiEvents::Data _assumeCh00[] = {
		0xf0, 0x00, 0x00,
		0x90, 0x00, 0x00, 0xf7,
		0x91, 0x91, 0x0, 
		0x81, 0x3C, 0x90,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents assumeCh00;
	assumeCh00.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(assumeCh00, b);
}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered06() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0xf0, 0x00, 0x00,
		0x90, 0x00, 0x00  // no sysex endbyte
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents a;
	a.events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents b;
	b.copyDeepFiltered(&a, 1);

	CPPUNIT_ASSERT_EQUAL(a, b);
}
} //namespace
