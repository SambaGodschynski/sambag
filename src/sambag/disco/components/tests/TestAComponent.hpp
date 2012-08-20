/*
 * TestComponent.hpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#ifndef TESTCOMPONENT_HPP_
#define TESTCOMPONENT_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <sambag/disco/components/Forward.hpp>


namespace tests {
//=============================================================================
class TestAComponent: public CPPUNIT_NS::TestFixture {
	//=============================================================================
private:
	CPPUNIT_TEST_SUITE( TestAComponent );
	CPPUNIT_TEST(test_contains);
	CPPUNIT_TEST(test_getPtr);
	CPPUNIT_TEST(test_bounds);
	CPPUNIT_TEST(test_toString);
	CPPUNIT_TEST(test_alignment);
	CPPUNIT_TEST(test_getComponentAt);
	CPPUNIT_TEST(test_minMaxSize);
	CPPUNIT_TEST(test_name);
	CPPUNIT_TEST(test_preferredSize);
	CPPUNIT_TEST(test_actionMap);
	CPPUNIT_TEST(test_autoscrolls);
	CPPUNIT_TEST(test_baseLine);
	CPPUNIT_TEST(test_border);
	CPPUNIT_TEST(test_grabFocus);
	CPPUNIT_TEST(test_getDrawContext);
	CPPUNIT_TEST(test_color);
	CPPUNIT_TEST(test_getTreeLock);
	CPPUNIT_TEST(test_ignoreRepaint);
	CPPUNIT_TEST(test_hasFocus);
	CPPUNIT_TEST(test_invalidate);
	CPPUNIT_TEST(test_isDisplayable);
	CPPUNIT_TEST(test_enabled);
	CPPUNIT_TEST(test_focusable);
	CPPUNIT_TEST(test_isFocusOwner);
	CPPUNIT_TEST(test_isOpaque);
	CPPUNIT_TEST(test_isShowing);
	CPPUNIT_TEST(test_isValid);
	CPPUNIT_TEST(test_visibility);
	CPPUNIT_TEST(test_requestFocus);
	CPPUNIT_TEST(test_revalidate);
	CPPUNIT_TEST(test_transferFocus);
	CPPUNIT_TEST(test_transferFocusBackward);
	CPPUNIT_TEST(test_validate);
	CPPUNIT_TEST(testPropertyChanged);
	CPPUNIT_TEST(testPutClientProperty);
	CPPUNIT_TEST(testGetFirstAndLastParent);
	CPPUNIT_TEST_SUITE_END();
private:
	sambag::disco::components::AComponentPtr comp;
public:
	void setUp();
	void tearDown();
	void test_contains();
	void test_getPtr();
	void test_bounds();
	void test_toString();
	void test_alignment();
	void test_getComponentAt();
	void test_minMaxSize();
	void test_name();
	void test_preferredSize();
	void test_actionMap();
	void test_autoscrolls();
	void test_baseLine();
	void test_border();
	void test_grabFocus();
	void test_getDrawContext();
	void test_color();
	void test_getTreeLock();
	void test_ignoreRepaint();
	void test_hasFocus();
	void test_invalidate();
	void test_isDisplayable();
	void test_enabled();
	void test_focusable();
	void test_isFocusOwner();
	void test_isOpaque();
	void test_isShowing();
	void test_isValid();
	void test_visibility();
	void test_requestFocus();
	void test_revalidate();
	void test_transferFocus();
	void test_transferFocusBackward();
	void test_validate();
	void testPropertyChanged();
	void testPutClientProperty();
	void testGetFirstAndLastParent();

};

} // namespace


#endif /* TESTCOMPONENT_HPP_ */
