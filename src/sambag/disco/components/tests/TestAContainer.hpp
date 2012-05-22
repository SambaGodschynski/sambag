/*
 * TestAContainer.hpp
 *
 *  Created on: Tue May 22 12:45:20 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTACONTAINER_H
#define SAMBAG_TESTACONTAINER_H

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/Forward.hpp>

namespace tests {
//=============================================================================
class TestAContainer : public CPPUNIT_NS::TestFixture {
//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestAContainer );
	CPPUNIT_TEST( testLayoutManager01 );
	CPPUNIT_TEST(test_add);
	CPPUNIT_TEST(test_doLayout);
	CPPUNIT_TEST(test_getComponentAt);
	CPPUNIT_TEST(test_alignment);
	CPPUNIT_TEST(test_getComponent);
	CPPUNIT_TEST(test_getComponentCount);
	CPPUNIT_TEST(test_getComponents);
	CPPUNIT_TEST(test_getComponentZOrder);
	CPPUNIT_TEST(test_getInsets);
	CPPUNIT_TEST(test_layout);
	CPPUNIT_TEST(test_minMaximumSize);
	CPPUNIT_TEST(test_preferredSize);
	CPPUNIT_TEST(test_isAncestorOf);
	CPPUNIT_TEST(test_isValidateRoot);
	CPPUNIT_TEST(test_invalidate);
	CPPUNIT_TEST(test_remove);
	CPPUNIT_TEST(test_removeAll);
	CPPUNIT_TEST(test_findComponentAt);
	CPPUNIT_TEST(test_toString);
	CPPUNIT_TEST_SUITE_END();
private:
	sambag::disco::components::AContainerPtr con;
public:
	void testLayoutManager01();
	void setUp();
	void tearDown();
	void test_add();
	void test_doLayout();
	void test_getComponentAt();
	void test_alignment();
	void test_getComponent();
	void test_getComponentCount();
	void test_getComponents();
	void test_getComponentZOrder();
	void test_getInsets();
	void test_layout();
	void test_minMaximumSize();
	void test_preferredSize();
	void test_isAncestorOf();
	void test_isValidateRoot();
	void test_invalidate();
	void test_remove();
	void test_removeAll();
	void test_findComponentAt();
	void test_toString();
};

} // namespace

#endif /* SAMBAG_TESTACONTAINER_H */
