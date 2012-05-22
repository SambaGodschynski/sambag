/*
 * TestFlowLayout.cpp
 *
 *  Created on: Wed May 30 07:19:36 2012
 *      Author: Johannes Unger
 */

#include "TestFlowLayout.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/AContainer.hpp>
#include <sambag/disco/components/FlowLayout.hpp>
#include "TestComponents.hpp"
#include "TestHelper.hpp"
#include <boost/shared_ptr.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestFlowLayout );

namespace tests {
//=============================================================================
//  Class TestFlowLayout
//=============================================================================
//-----------------------------------------------------------------------------
void TestFlowLayout::setUp() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	con = TestContainer::create();
	con->setName("TestFlowLayoutContainer");
	con->setBounds(Rectangle(0, 0, 500, 550));
	con->setLayout( FlowLayout::create() );
	// init components
	for (int i = 0; i < NUM_COMPOS; ++i) {
		std::stringstream ss;
		ss << "TestComponent" << i;
		comps[i] = TestComponent::create();
		comps[i]->setName(ss.str());
		comps[i]->setBounds(Rectangle(0,0,50,55));
		con->add(comps[i]);
	}
}
//-----------------------------------------------------------------------------
void TestFlowLayout::testLayout() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	static const std::string soll("[TestComponent0,115,5,50x55,invalid]\n"
			"[TestComponent1,170,5,50x55,invalid]\n"
			"[TestComponent2,225,5,50x55,invalid]\n"
			"[TestComponent3,280,5,50x55,invalid]\n"
			"[TestComponent4,335,5,50x55,invalid]\n");
	static const std::string soll02("[TestComponent0,22.5,5,50x55,invalid]\n"
			"[TestComponent1,77.5,5,50x55,invalid]\n"
			"[TestComponent2,22.5,65,50x55,invalid]\n"
			"[TestComponent3,77.5,65,50x55,invalid]\n"
			"[TestComponent4,50,125,50x55,invalid]\n");
	std::stringstream ss;
	con->doLayout();
	for (int i = 0; i < NUM_COMPOS; ++i) {
		AComponent::Ptr cm = con->getComponent(i);
		ss<<cm->toString()<<std::endl;
	}
	CPPUNIT_ASSERT_EQUAL( soll, ss.str() );
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<resize components
	for (int i = 0; i < NUM_COMPOS; ++i) {
		AComponent::Ptr cm = con->getComponent(i);
		con->setSize(150, 100);
	}
	ss.str("");
	ss.clear();
	con->doLayout();
	for (int i = 0; i < NUM_COMPOS; ++i) {
		AComponent::Ptr cm = con->getComponent(i);
		ss<<cm->toString()<<std::endl;
	}
	CPPUNIT_ASSERT_EQUAL( soll02, ss.str() );
}
//-----------------------------------------------------------------------------
void TestFlowLayout::testAlignment() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	static const std::string sollLeft("[TestComponent0,0,5,50x55,invalid]\n"
			"[TestComponent1,55,5,50x55,invalid]\n"
			"[TestComponent2,110,5,50x55,invalid]\n"
			"[TestComponent3,165,5,50x55,invalid]\n"
			"[TestComponent4,220,5,50x55,invalid]\n");
	static const std::string sollRight("[TestComponent0,220,5,50x55,invalid]\n"
			"[TestComponent1,275,5,50x55,invalid]\n"
			"[TestComponent2,330,5,50x55,invalid]\n"
			"[TestComponent3,385,5,50x55,invalid]\n"
			"[TestComponent4,440,5,50x55,invalid]\n");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<LEFT
	std::stringstream ss;
	FlowLayout::Ptr fl = boost::shared_dynamic_cast<FlowLayout>(con->getLayout());
	CPPUNIT_ASSERT(fl);
	fl->setAlignment(FlowLayout::LEFT);
	con->doLayout();
	for (int i = 0; i < NUM_COMPOS; ++i) {
		AComponent::Ptr cm = con->getComponent(i);
		ss<<cm->toString()<<std::endl;
	}
	CPPUNIT_ASSERT_EQUAL( sollLeft, ss.str() );
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<RIGHT
	ss.str("");
	ss.clear();
	fl->setAlignment(FlowLayout::RIGHT);
	con->doLayout();
	for (int i = 0; i < NUM_COMPOS; ++i) {
		AComponent::Ptr cm = con->getComponent(i);
		ss<<cm->toString()<<std::endl;
	}
	CPPUNIT_ASSERT_EQUAL( sollRight, ss.str() );
}
//-----------------------------------------------------------------------------
void TestFlowLayout::testGap() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	static const std::string soll("[TestComponent0,125,50,50x55,invalid]\n"
			"[TestComponent1,325,50,50x55,invalid]\n"
			"[TestComponent2,125,155,50x55,invalid]\n"
			"[TestComponent3,325,155,50x55,invalid]\n"
			"[TestComponent4,225,260,50x55,invalid]\n");
	FlowLayout::Ptr fl = boost::shared_dynamic_cast<FlowLayout>(con->getLayout());
	CPPUNIT_ASSERT(fl);
	fl->setHgap(150);
	fl->setVgap(50);
	std::stringstream ss;
	con->doLayout();
	for (int i = 0; i < NUM_COMPOS; ++i) {
		AComponent::Ptr cm = con->getComponent(i);
		ss<<cm->toString()<<std::endl;
	}
	CPPUNIT_ASSERT_EQUAL( soll, ss.str() );
}
//-----------------------------------------------------------------------------
void TestFlowLayout::testPreferredLayoutSize() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	FlowLayout::Ptr fl = boost::shared_dynamic_cast<FlowLayout>(con->getLayout());
	CPPUNIT_ASSERT(fl);
	CPPUNIT_ASSERT_EQUAL( Dimension(280, 65), fl->preferredLayoutSize(con));
}
//-----------------------------------------------------------------------------
void TestFlowLayout::testMinimumLayoutSize() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	FlowLayout::Ptr fl = boost::shared_dynamic_cast<FlowLayout>(con->getLayout());
	CPPUNIT_ASSERT(fl);
	CPPUNIT_ASSERT_EQUAL( Dimension(280, 290), fl->minimumLayoutSize(con));
}
//-----------------------------------------------------------------------------
void TestFlowLayout::testToString() {
	static const std::string soll("FlowLayout [hgap=5,vgap=5,align=center]");
	CPPUNIT_ASSERT_EQUAL(soll, con->getLayout()->toString());
}
} //namespace
