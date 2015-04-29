/*
 * TestVst3MidiEvents.hpp
 *
 *  Created on: Mon Feb 16 21:10:49 2015
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTVST3MIDIEVENTS_H
#define SAMBAG_TESTVST3MIDIEVENTS_H

#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
class TestVst3MidiEvents : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestVst3MidiEvents );
	CPPUNIT_TEST( testNoteOffBug );
    CPPUNIT_TEST( testHandleNull );
	CPPUNIT_TEST_SUITE_END();
public:
	void testNoteOffBug();
    void testHandleNull();
};

} // namespace

#endif /* SAMBAG_TESTVST3MIDIEVENTS_H */
