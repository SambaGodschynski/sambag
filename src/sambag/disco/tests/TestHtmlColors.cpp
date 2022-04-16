/*
 * TestHtmlColors.cpp
 *
 *  Created on: Tue Aug 12 15:53:20 2014
 *      Author: Johannes Unger
 */

#include "TestHtmlColors.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/svg/HtmlColors.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestHtmlColors );

namespace tests {
//=============================================================================
//  Class TestHtmlColors
//=============================================================================
//-----------------------------------------------------------------------------
void TestHtmlColors::testToRGB() {
    using namespace sambag::disco;
    CPPUNIT_ASSERT_EQUAL(std::string("#ffffff"), svg::HtmlColors::toRGB(ColorRGBA(1,1,1)));
    CPPUNIT_ASSERT_EQUAL(std::string("#00ffff"), svg::HtmlColors::toRGB(ColorRGBA(0,1,1)));
    CPPUNIT_ASSERT_EQUAL(std::string("#0000ff"), svg::HtmlColors::toRGB(ColorRGBA(0,0,1)));
    CPPUNIT_ASSERT_EQUAL(std::string("#000000"), svg::HtmlColors::toRGB(ColorRGBA(0,0,0)));
    CPPUNIT_ASSERT_EQUAL(std::string("#7f7f7f"), svg::HtmlColors::toRGB(ColorRGBA(0.5,0.5,0.5)));
    CPPUNIT_ASSERT_EQUAL(std::string("#ffffff"), svg::HtmlColors::toRGB(ColorRGBA(2,2,2)));
    CPPUNIT_ASSERT_EQUAL(std::string("#000000"), svg::HtmlColors::toRGB(ColorRGBA(-1,-1,-1)));
}
} //namespace
