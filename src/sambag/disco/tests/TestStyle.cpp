/*
 * TestStyle.cpp
 *
 *  Created on: Tue Aug 12 17:34:14 2014
 *      Author: Johannes Unger
 */

#include "TestStyle.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/svg/Style.hpp>
#include <sambag/disco/svg/StyleParser.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestStyle );

namespace tests {
//=============================================================================
//  Class TestStyle
//=============================================================================
//-----------------------------------------------------------------------------
void TestStyle::testToString() {
    using namespace sambag::disco::svg;
    using namespace sambag::io;
    Style style;
    CPPUNIT_ASSERT_EQUAL(std::string("stroke: none; stroke-width: none; stroke-opacity: none; stroke-dasharray: none; \
fill: none; fill-opacity: none; font-family: none; font-size: none; font-weight\
: none; font-style: none;"), style.toString());
    std::stringstream ss;
    ss<<"stroke: red; stroke-width: 2; stroke-opacity: 0.5; stroke-dasharray: 10 4 10; stroke-dashoffset: 2; \
fill: blue; fill-opacity: 0.2; font-family: arial; font-size: 21; font-weight\
: bold; font-style: italic;";
    ss>>style;
    CPPUNIT_ASSERT_EQUAL(std::string("stroke: #ff0000; stroke-width: 2; stroke-opacity: 0.5; stroke-dasharray: 10 4 10; stroke-dashoffset: 2; \
fill: #0000ff; fill-opacity: 0.2; font-family: arial; font-size: 21; font-weight\
: bold; font-style: italic;"), style.toString());
}
} //namespace
