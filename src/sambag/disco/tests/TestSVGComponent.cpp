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
        <g class='Disco' id='A'>                                            \
            <g class='Disco' id='B'>                                        \
                <g class='Disco' id='C'>                                    \
                </g>                                                        \
            </g>                                                            \
            <g class='DISCO' id='D'>                                        \
            </g>                                                            \
        </g>                                                                \
    </svg>");
    svg->setSize(Dimension(800,600));
    svg->doLayout();
    svg->printComponentTree(std::cout);
}
} //namespace
