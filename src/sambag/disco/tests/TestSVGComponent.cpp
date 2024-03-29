/*
 * TestSVGComponent.cpp
 *
 *  Created on: Sun Apr 13 20:38:40 2014
 *      Author: Johannes Unger
 */

#include "TestSVGComponent.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/SvgComponent.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestSVGComponent );

namespace tests {
//=============================================================================
//  Class TestSVGComponent
//=============================================================================
//-----------------------------------------------------------------------------
void TestSVGComponent::testComponentTree() {
    using namespace sambag::disco;
	using namespace sambag::disco::components;

    SvgComponent::Ptr svg = SvgComponent::create();
    svg->setSvgString("<svg>                                                \
        <g class='disco' id='A'>                                            \
            <g class='disco' id='B'>                                        \
                <g class='disco' id='C'>                                    \
                </g>                                                        \
            </g>                                                            \
            <g class='disco' id='D'>                                        \
            </g>                                                            \
        </g>                                                                \
    </svg>");
    svg->setSize(Dimension(800,600));
    svg->doLayout();
    std::stringstream ss;
    svg->printComponentTree(ss);
    CPPUNIT_ASSERT_EQUAL(std::string("[SvgComponent,0,0,800x600,invalid,alignmentX=0.5,alignmentY=0.5,max\
imumSize=Rectangle(Point2D(0, 0), Point2D(0, 0))]\n\
 [<g id='#C' class='.disco '/>,-1,-1,2x2,invalid,alignmentX=0.5,alignmentY=0.5]\n\
 [<g id='#B' class='.disco '/>,-1,-1,2x2,invalid,alignmentX=0.5,alignmentY=0.5]\n\
 [<g id='#D' class='.disco '/>,-1,-1,2x2,invalid,alignmentX=0.5,alignmentY=0.5]\n\
 [<g id='#A' class='.disco '/>,-1,-1,2x2,invalid,alignmentX=0.5,alignmentY=0.5]\n"), ss.str());
    
}
} //namespace
