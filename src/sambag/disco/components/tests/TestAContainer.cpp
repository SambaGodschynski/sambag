/*
 * TestAContainer.cpp
 *
 *  Created on: Tue May 22 12:45:20 2012
 *      Author: Johannes Unger
 */

#include "TestAContainer.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/AContainer.hpp>
#include "TestComponents.hpp"
#include "TestHelper.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include <sambag/disco/components/FlowLayout.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestAContainer );


namespace tests {
//=============================================================================
//  Class TestAContainer
//=============================================================================
//-----------------------------------------------------------------------------
void TestAContainer::setUp() {
	using namespace sambag::disco;
	con = TestContainer::create();
	con->setName("TestContainer");
	con->setBounds(Rectangle(0, 0, 500, 550));
	con->setLayout(components::FlowLayout::create());
}
//-----------------------------------------------------------------------------
void TestAContainer::tearDown() {

}
//-----------------------------------------------------------------------------
void TestAContainer::test_toString() {
	std::string str("[TestContainer,0,0,500x550,invalid,alignmentX=0.5,alignmentY=0.5]");
	CPPUNIT_ASSERT_EQUAL(str, con->toString());
}
//-----------------------------------------------------------------------------
void TestAContainer::test_printComponentTree() {
	using namespace sambag::disco::components;
	static const size_t NUM = 3;
	for (size_t i=0; i<NUM; ++i) {
		TestComponent::Ptr comp = TestComponent::create();
		comp->setSize(50, 50);
		std::stringstream ss;
		ss << "Test Component Nr. " << i;
		comp->setName(ss.str());
		con->add(comp);
	}
	con->validate();
	std::stringstream ss;
	ss << *con.get();
	std::string exp("[TestContainer,0,0,500x550,alignmentX=0.5,alignmentY=0.5]\n"
			" [Test Component Nr. 0,170,5,50x50]\n"
			" [Test Component Nr. 1,225,5,50x50]\n"
			" [Test Component Nr. 2,280,5,50x50]\n");
	CPPUNIT_ASSERT_EQUAL(exp, ss.str());
}
//-----------------------------------------------------------------------------
void TestAContainer::test_add() {
	using namespace sambag::disco::components;
	static const size_t NUM = 3;
	AComponent::Ptr components[NUM];
	for (size_t i=0; i<NUM; ++i) {
		components[i] = TestComponent::create();
		components[i]->setSize(50, 50);
		std::stringstream ss;
		ss << "Test Component Nr. " << i;
		components[i]->setName(ss.str());
	}
	CPPUNIT_ASSERT_EQUAL((size_t)0, con->getComponentCount());
	con->add(components[0]);
	CPPUNIT_ASSERT_EQUAL((size_t)1, con->getComponentCount());
	con->add(components[1], 0);
	CPPUNIT_ASSERT_EQUAL((size_t)2, con->getComponentCount());
	CPPUNIT_ASSERT(components[1] == con->getComponent(0));
	CPPUNIT_ASSERT(components[0] == con->getComponent(1));
	CPPUNIT_ASSERT_THROW(con->add(components[2], 3),
			sambag::com::exceptions::IllegalArgumentException);

}
//-----------------------------------------------------------------------------
void TestAContainer::test_doLayout() {
	// see Layoutmanager tests
}
//-----------------------------------------------------------------------------
void TestAContainer::test_getComponentAt() {
	using namespace sambag::disco::components;
	static const size_t NUM = 2;
	AComponent::Ptr components[NUM];
	for (size_t i=0; i<NUM; ++i) {
		components[i] = TestComponent::create();
		components[i]->setSize(50, 55);
		std::stringstream ss;
		ss << "Test Component Nr. " << i;
		components[i]->setName(ss.str());
		con->add(components[i]);
	}
	con->doLayout();
	CPPUNIT_ASSERT(con == con->getComponentAt(0,0));
	CPPUNIT_ASSERT(components[0] == con->getComponentAt(198,10));
	CPPUNIT_ASSERT(components[1] == con->getComponentAt(253,10));
	CPPUNIT_ASSERT(!con->getComponentAt(1000,1000));
}
//-----------------------------------------------------------------------------
void TestAContainer::test_alignment() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_getComponent() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_getComponentCount() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_getComponents() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_getComponentZOrder() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_getInsets() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_layout() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_minMaximumSize() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_preferredSize() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_isAncestorOf() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_isValidateRoot() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_invalidate() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_remove() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_removeAll() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAContainer::test_findComponentAt() {
	testMissing();
}
} //namespace
