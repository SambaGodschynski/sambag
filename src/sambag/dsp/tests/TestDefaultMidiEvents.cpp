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
void TestDefaultMidiEvents::testInsertDeep() {
	using namespace sambag::dsp;
	typedef boost::shared_array<IMidiEvents::Data> DataArray;
    typedef boost::shared_ptr<IMidiEvents> MidiEventsPtr;
	DataArray statData[1024];
	DataArray tmp[1024];
    DefaultMidiEvents deep;
    DefaultMidiEvents stat;
	for (int i=0; i<1024; ++i) {
		int bytes = (i%15) + 1;
		statData[i] = DataArray(new IMidiEvents::Data[bytes]);
        tmp[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			statData[i][j] = j % 26 + 'a';
            tmp[i][j] = j % 26 + 'a';
		}
        // use the static data
        stat.insertFlat(IMidiEvents::MidiEvent(bytes, i, statData[i].get()));
        // use the tmp data
		deep.insertDeep(IMidiEvents::MidiEvent(bytes, i, tmp[i].get()));
	}
    // clear tmp
    for (int i=0; i<1024; ++i) {
        int bytes = (i%15) + 1;
        memset(tmp[i].get(), 0, bytes);
    }
    CPPUNIT_ASSERT(stat==deep);
}

//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testCreateFlatRawData() {
	using namespace sambag::dsp;
	typedef boost::shared_array<IMidiEvents::Data> DataArray;
    typedef boost::shared_ptr<IMidiEvents> MidiEventsPtr;
	DataArray data[1024];
	DefaultMidiEvents orig;
    size_t sumBytes = 0;
	for (int i=0; i<1024; ++i) {
		int bytes = (i%15) + 1;
		data[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data[i][j] = j % 26 + 'a';
		}
        sumBytes+=bytes+sizeof(IMidiEvents::ByteSize)+sizeof(IMidiEvents::DeltaFrames);
		orig.insertFlat(IMidiEvents::MidiEvent(bytes, i, data[i].get()));
	}
	DataArray bff;
    size_t size = 0;
    {
        std::vector<IMidiEvents::Data> tmp;
        tmp.reserve(size);
        createFlatRawData(orig, tmp);
        size = tmp.size();
        CPPUNIT_ASSERT_EQUAL(size, sumBytes);
        bff = DataArray( new IMidiEvents::Data[size] );
        memcpy(bff.get(), &tmp[0], sizeof(IMidiEvents::Data) * size);
    }
    MidiEventsPtr newEvent = MidiEventsPtr( createMidiEvents(bff.get(), size) );
    CPPUNIT_ASSERT(orig==(*newEvent));
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
