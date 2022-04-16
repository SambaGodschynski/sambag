/*
 * TestConcreteComponents.hpp
 *
 *  Created on: Tue Jun  5 07:30:24 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTCONCRETECOMPONENTS_H
#define SAMBAG_TESTCONCRETECOMPONENTS_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/com/UnitEx.hpp>
#include <sambag/disco/components/windowImpl/WindowTestToolkit.hpp>
#include <sambag/disco/components/Window.hpp>

namespace tests {
//=============================================================================
class TestConcreteComponents : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestConcreteComponents );
	CPPUNIT_TEST_KNOWN_ISSUE( testButton );
	CPPUNIT_TEST_SUITE_END();
	sambag::disco::components::RootPane::Ptr root;
	sambag::disco::components::AContainer::Ptr cont;
	sambag::disco::IImageSurface::Ptr surf;
	sambag::disco::components::Window::Ptr win;
	sambag::disco::components::WindowTestToolkit<>::Ptr wfac;
public:
	void setUp();
	void testButton();

};

} // namespace

#endif /* SAMBAG_TESTCONCRETECOMPONENTS_H */
