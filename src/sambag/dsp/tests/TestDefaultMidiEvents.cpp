/*
 * TestDefaultMidiEvents.cpp
 *
 *  Created on: Fri Nov 30 14:19:00 2012
 *      Author: Johannes Unger
 */

#include "TestDefaultMidiEvents.hpp"
#include <sambag/dsp/DefaultMidiEvents.hpp>
#include <cppunit/config/SourcePrefix.h>
#include <cstring>

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
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	for (int i=0; i<1024; ++i) {
		int bytes = (i%15) + 1;
		data[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data[i][j] = j % 26 + 'a';
		}
		a->events.push_back(IMidiEvents::MidiEvent(bytes, i, data[i].get()));
	}
	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create(a); // flat copy
	CPPUNIT_ASSERT(*a==*b);
	DefaultMidiEvents::Ptr c = DefaultMidiEvents::create();
	c->copyDeep(a);
	CPPUNIT_ASSERT(*c==*a);
	data[0][0] = '!';
	CPPUNIT_ASSERT(*c!=*a);
	CPPUNIT_ASSERT(*c!=*b);
	CPPUNIT_ASSERT(*a==*b);
}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testInsertDeep() {
	using namespace sambag::dsp;
	typedef boost::shared_array<IMidiEvents::Data> DataArray;
    typedef boost::shared_ptr<IMidiEvents> MidiEventsPtr;
	DataArray statData[1024];
	DataArray tmp[1024];
    DefaultMidiEvents::Ptr deep = DefaultMidiEvents::create();
    DefaultMidiEvents::Ptr stat = DefaultMidiEvents::create();
	for (int i=0; i<1024; ++i) {
		int bytes = (i%15) + 1;
		statData[i] = DataArray(new IMidiEvents::Data[bytes]);
        tmp[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			statData[i][j] = j % 26 + 'a';
            tmp[i][j] = j % 26 + 'a';
		}
        // use the static data
        stat->insertFlat(IMidiEvents::MidiEvent(bytes, i, statData[i].get()));
        // use the tmp data
		deep->insertDeep(IMidiEvents::MidiEvent(bytes, i, tmp[i].get()));
	}
    // clear tmp
    for (int i=0; i<1024; ++i) {
        int bytes = (i%15) + 1;
        memset(tmp[i].get(), 0, bytes);
    }
    CPPUNIT_ASSERT(*stat==*deep);
}

//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testCreateFlatRawData() {
	using namespace sambag::dsp;
	typedef boost::shared_array<IMidiEvents::Data> DataArray;
    typedef boost::shared_ptr<IMidiEvents> MidiEventsPtr;
	DataArray data[1024];
	DefaultMidiEvents::Ptr orig = DefaultMidiEvents::create();
    size_t sumBytes = 0;
	for (int i=0; i<1024; ++i) {
		int bytes = (i%15) + 1;
		data[i] = DataArray(new IMidiEvents::Data[bytes]);
		for (int j=0; j<bytes; ++j) {
			data[i][j] = j % 26 + 'a';
		}
        sumBytes+=bytes+sizeof(IMidiEvents::ByteSize)+sizeof(IMidiEvents::DeltaFrames);
		orig->insertFlat(IMidiEvents::MidiEvent(bytes, i, data[i].get()));
	}
	DataArray bff;
    size_t size = 0;
    {
        std::vector<IMidiEvents::Data> tmp;
        tmp.reserve(size);
        createFlatRawData(*orig, tmp);
        size = tmp.size();
        CPPUNIT_ASSERT_EQUAL(size, sumBytes);
        bff = DataArray( new IMidiEvents::Data[size] );
        memcpy(bff.get(), &tmp[0], sizeof(IMidiEvents::Data) * size);
    }
    MidiEventsPtr newEvent = MidiEventsPtr( createMidiEvents(bff.get(), size) );
    CPPUNIT_ASSERT(*orig==(*newEvent));
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
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	a->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create();
	b->copyDeepFiltered(a, 0);

	IMidiEvents::Data _assumeCh00[] = {
		0x90, 0x3C, 0x87, 
		0x80, 0x3C, 0x0,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents::Ptr assumeCh00 = DefaultMidiEvents::create();
	assumeCh00->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(*assumeCh00, *b);

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
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	a->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create();
	b->copyDeepFiltered(a, 0);

	IMidiEvents::Data _assumeCh00[] = {
		0x00, 0x00, 0x00,
		0x90, 0x3C, 0x87, 
		0x80, 0x3C, 0x0,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents::Ptr assumeCh00 = DefaultMidiEvents::create();
	assumeCh00->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(*assumeCh00, *b);

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
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	a->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create();
	b->copyDeepFiltered(a, 1);

	IMidiEvents::Data _assumeCh00[] = {
		0x91, 0x91, 0x0, 
		0x81, 0x3C, 0x90,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents::Ptr assumeCh00 = DefaultMidiEvents::create();
	assumeCh00->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(*assumeCh00, *b);
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
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	a->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create();
	b->copyDeepFiltered(a, 1);

	IMidiEvents::Data _assumeCh00[] = {
		0x91, 0x91, 0x0, 
		0x81, 0x3C, 0x90,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents::Ptr assumeCh00 = DefaultMidiEvents::create();
	assumeCh00->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(*assumeCh00, *b);
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
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	a->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create();
	b->copyDeepFiltered(a, 1);

	IMidiEvents::Data _assumeCh00[] = {
		0xf0, 0x00, 0x00,
		0x90, 0x00, 0x00, 0xf7,
		0x91, 0x91, 0x0, 
		0x81, 0x3C, 0x90,
	};
	bytes = sizeof(_assumeCh00) / sizeof(_assumeCh00[0]);
	DefaultMidiEvents::Ptr assumeCh00 = DefaultMidiEvents::create();
	assumeCh00->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &_assumeCh00[0]));

	CPPUNIT_ASSERT_EQUAL(*assumeCh00, *b);
}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testEventCopyFiltered06() {
	using namespace sambag::dsp;
	IMidiEvents::Data data[] = {
		0xf0, 0x00, 0x00,
		0x90, 0x00, 0x00  // no sysex endbyte
	};
	size_t bytes = sizeof(data) / sizeof(data[0]);
	DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
	a->events.push_back(IMidiEvents::MidiEvent(bytes, 1, &data[0]));

	DefaultMidiEvents::Ptr b = DefaultMidiEvents::create();
	b->copyDeepFiltered(a, 1);

	CPPUNIT_ASSERT_EQUAL(*a, *b);
}
//-----------------------------------------------------------------------------
void TestDefaultMidiEvents::testHelper()
{
    using namespace sambag::dsp;
    {
        IMidiEvents::Data data[] = { 0x81, 100, 99 };
        size_t bytes = 3;
        DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 1, &data[0]));
        CPPUNIT_ASSERT_EQUAL(IMidiEvents::NoteOff, getEventType(a->getMidiEvent(0)));
        CPPUNIT_ASSERT_EQUAL(1, getChannel(a->getMidiEvent(0)));
        CPPUNIT_ASSERT_EQUAL(100, getPitch(a->getMidiEvent(0)));
        CPPUNIT_ASSERT_EQUAL(99, getVelocity(a->getMidiEvent(0)));
    }
    {
        IMidiEvents::Data data[] = { 0xEA, 10, 99 };
        size_t bytes = 3;
        DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 1, &data[0]));
        CPPUNIT_ASSERT_EQUAL(IMidiEvents::PitchBend, getEventType(a->getMidiEvent(0)));
        CPPUNIT_ASSERT_EQUAL(10, getChannel(a->getMidiEvent(0)));
        CPPUNIT_ASSERT_EQUAL(99*128+10, getPitchBend(a->getMidiEvent(0)));
    }
    
}
//-----------------------------------------------------------------------------
namespace {
    inline void testByte(sambag::dsp::MidiDataIterator &it,
        unsigned char byte, int delta)
    {
        unsigned char ist = 0;
        CPPUNIT_ASSERT( it.nextByte(&ist) );
        CPPUNIT_ASSERT_EQUAL(byte, ist);
        CPPUNIT_ASSERT_EQUAL(delta, it.deltaFrames());
    }
}
void TestDefaultMidiEvents::testMidiDataIterator() {
    using namespace sambag::dsp;
    DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
    MidiDataIterator it(a);
    IMidiEvents::Data byte;
    CPPUNIT_ASSERT( !it.nextByte(&byte) );
    CPPUNIT_ASSERT( !it.nextByte(&byte) );
    // create data
    {
        IMidiEvents::Data data[] = { 0x90, 0x3C, 0x22 };
        a->insertDeep(IMidiEvents::MidiEvent(3, 1, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x91, 0x3D, 0x23 };
        a->insertDeep(IMidiEvents::MidiEvent(3, 10, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x0 };
        a->insertDeep(IMidiEvents::MidiEvent(1, 0, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x92, 0x3E, 0x24 };
        a->insertDeep(IMidiEvents::MidiEvent(3, 100, &data[0]));
    }
    testByte(it, 0x90, 1); testByte(it, 0x3C, 1); testByte(it, 0x22, 1);
    testByte(it, 0x91, 10); testByte(it, 0x3D, 10); testByte(it, 0x23, 10);
    testByte(it, 0x0, 0);
    testByte(it, 0x92, 100); testByte(it, 0x3E, 100); testByte(it, 0x24, 100);
    CPPUNIT_ASSERT( !it.nextByte(&byte) );
	
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
void TestDefaultMidiEvents::testTrim() {
	using namespace sambag::dsp;
    CPPUNIT_ASSERT( !trim(IMidiEvents::Ptr()) );
    DefaultMidiEvents::Ptr a = DefaultMidiEvents::create();
    CPPUNIT_ASSERT( trim(a)->getNumEvents() == 0 );
    // create data stream splitted to several events
    {
        IMidiEvents::Data data[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 0, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x00, 0x00, 0x00, 0x90, 0x10, 0x50, 0x00, 0x00 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 1, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x00, 0x00 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 2, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x00, 0x80, 0x20, 0x30, 0xF2, 0xA0, 0x81, 0xF3, 0xE0, 0xD1, 0x33, 0x00, 0x00 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 3, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x00, 0xB0, 0x01 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 4, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x10, 0xE0, 0x11, 0x22 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 5, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0xF0, 0x00, 0x00 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 6, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0x00, 0x00, 0x00, 0xF7 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 7, &data[0]));
    }
    {
        IMidiEvents::Data data[] = { 0xF0, 0xF7 };
        size_t bytes = sizeof(data) / sizeof(data[0]);
        a->insertDeep(IMidiEvents::MidiEvent(bytes, 1, &data[0]));
    }
    IMidiEvents::Ptr b = trim(a);
    CPPUNIT_ASSERT_EQUAL((int)7, b->getNumEvents());
    testEventData(b, 0, 1, 3, 0x90, 0x10, 0x50);
    testEventData(b, 1, 3, 3, 0x80, 0x20, 0x30);
    testEventData(b, 2, 3, 2, 0xD1, 0x33);
    testEventData(b, 3, 4, 3, 0xB0, 0x01, 0x10);
    testEventData(b, 4, 5, 3, 0xE0, 0x11, 0x22);
    testEventData(b, 5, 6, 7, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7);
    testEventData(b, 6, 1, 2, 0xF0, 0xF7);
    
}
} //namespace
