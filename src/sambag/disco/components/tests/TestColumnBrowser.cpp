/*
 * TestColumnBrowser.cpp
 *
 *  Created on: Thu Mar 13 15:04:40 2014
 *      Author: Johannes Unger
 */

#include "TestColumnBrowser.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/ColumnBrowser.hpp>
#include <string>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestColumnBrowser );

namespace tests {
//=============================================================================
//  Class TestColumnBrowser
//=============================================================================
//-----------------------------------------------------------------------------
void TestColumnBrowser::testSelectPath() {
    typedef sambag::disco::components::ColumnBrowser<std::string> Browser;
	Browser::Ptr miller = Browser::create();
	Browser::Node A = miller->addNode(miller->getRootNode(), "A");
	Browser::Node B = miller->addNode(miller->getRootNode(), "B");
	Browser::Node A1 = miller->addNode(A, "A1");
    miller->addNode(A1, "A11");
    miller->addNode(B, "B1");
	miller->setSelectionPath("A");
    CPPUNIT_ASSERT_EQUAL(std::string("A"), miller->selectionPathToString());
	miller->setSelectionPath("A/A1/A11");
    CPPUNIT_ASSERT_EQUAL(std::string("A/A1/A11"), miller->selectionPathToString());
    miller->setSelectionPath("B/B1");
    CPPUNIT_ASSERT_EQUAL(std::string("B/B1"), miller->selectionPathToString());
    miller->setSelectionPath("A/X");
    CPPUNIT_ASSERT_EQUAL(std::string("A"), miller->selectionPathToString());
    miller->setSelectionPath("X");
    CPPUNIT_ASSERT_EQUAL(std::string(""), miller->selectionPathToString());
}
} //namespace
