/*
 * TestAComponent.cpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#include "TestAComponent.hpp"
#include "TestComponents.hpp"
#include <sambag/disco/components/AComponent.hpp>
#include <sambag/disco/components/AContainer.hpp>
#include <sambag/disco/Geometry.hpp>
#include "TestHelper.hpp"
#include <sstream>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestAComponent );

namespace tests {
//=============================================================================
// TestAComponent
//=============================================================================
//-----------------------------------------------------------------------------
void TestAComponent::testPropertyChanged() {
	using namespace sambag::disco;
	using namespace sambag::com::events;
	using namespace sambag::disco::components;
	{
		TestComponent::Ptr dmy = TestComponent::create();
		PropertyTestListener<std::string>
		testListener(AComponent::PROPERTY_NAME, "", "xxx");
		dmy->EventSender<PropertyChanged>::addTrackedEventListener(
				boost::bind(&PropertyTestListener<std::string>::propertyChanged,
				&testListener, _1, _2), dmy
		);
		// comp->setName(""); no prop.ch. ever called
		CPPUNIT_ASSERT(!testListener.wasCalled);
	}
	{
		TestComponent::Ptr dmy = TestComponent::create();
		PropertyTestListener<std::string>
		testListener("wrong property", "", "xxx");
		dmy->EventSender<PropertyChanged>::addTrackedEventListener(
				boost::bind(&PropertyTestListener<std::string>::propertyChanged,
				&testListener, _1, _2), dmy
		);
		comp->setName("xxx");
		CPPUNIT_ASSERT(!testListener.wasCalled);
	}
	{
		TestComponent::Ptr dmy = TestComponent::create();
		PropertyTestListener<std::string>
		testListener(AComponent::PROPERTY_NAME, "wrong value", "xxx");
		dmy->EventSender<PropertyChanged>::addTrackedEventListener(
				boost::bind(&PropertyTestListener<std::string>::propertyChanged,
				&testListener, _1, _2), dmy
		);
		dmy->setName("xxx");
		CPPUNIT_ASSERT(testListener.wasCalled);
		CPPUNIT_ASSERT(testListener.createValidCopy() != testListener);
	}
	{
		TestComponent::Ptr dmy = TestComponent::create();
		PropertyTestListener<std::string>
		testListener(AComponent::PROPERTY_NAME, "", "xxx");
		dmy->EventSender<PropertyChanged>::addTrackedEventListener(
				boost::bind(&PropertyTestListener<std::string>::propertyChanged,
				&testListener, _1, _2), dmy
		);
		dmy->setName("wrong new value");
		CPPUNIT_ASSERT(testListener.createValidCopy() != testListener);
	}
	TestComponent::Ptr dmy = TestComponent::create();
	PropertyTestListener<std::string>
	testListener(AComponent::PROPERTY_NAME, "", "xxx");
	dmy->EventSender<PropertyChanged>::addEventListener(
			boost::bind(&PropertyTestListener<std::string>::propertyChanged,
			&testListener, _1, _2)
	);
	dmy->setName("xxx"); //o.k.
	CPPUNIT_ASSERT_EQUAL(
			testListener.createValidCopy(),
			testListener
	);
}
//-----------------------------------------------------------------------------
void TestAComponent::setUp() {
	using namespace sambag::disco;
	comp = TestComponent::create();
	comp->setName("TestComponent");
	comp->setBounds(sambag::disco::Rectangle(0, 0, 50, 55));
}
//-----------------------------------------------------------------------------
void TestAComponent::tearDown() {
}
//-----------------------------------------------------------------------------
void TestAComponent::test_contains() {
	CPPUNIT_ASSERT(comp->contains(10, 15));
	CPPUNIT_ASSERT(comp->contains(0, 0));
	CPPUNIT_ASSERT(!comp->contains(-1, -1));
	CPPUNIT_ASSERT(comp->contains(49, 54));
	CPPUNIT_ASSERT(!comp->contains(50, 55));
}
//-----------------------------------------------------------------------------
void TestAComponent::test_getPtr() {
	CPPUNIT_ASSERT(comp->getPtr());
}
//-----------------------------------------------------------------------------
void TestAComponent::test_bounds() {
	using namespace sambag::disco::components;
	using namespace sambag::disco;
	CPPUNIT_ASSERT_EQUAL( sambag::disco::Rectangle(0, 0, 50, 55), comp->getBounds() );
	comp->setSize(Dimension(230,200));
	comp->setLocation(Point2D(0, 0));
	CPPUNIT_ASSERT_EQUAL( sambag::disco::Rectangle(0, 0, 230, 200), comp->getBounds() );
	comp->setLocation(Point2D(10, 10));
	CPPUNIT_ASSERT_EQUAL( sambag::disco::Rectangle(10, 10, 230, 200), comp->getBounds() );
}
//-----------------------------------------------------------------------------
void TestAComponent::test_toString() {
	std::string str("[TestComponent,0,0,50x55,invalid]");
	CPPUNIT_ASSERT_EQUAL(str, comp->toString());
}
//-----------------------------------------------------------------------------
void TestAComponent::test_alignment() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_getComponentAt() {
	CPPUNIT_ASSERT(comp == comp->getComponentAt(0,0));
	CPPUNIT_ASSERT(!comp->getComponentAt(100,100));
}
//-----------------------------------------------------------------------------
void TestAComponent::test_minMaxSize() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	PropertyTestListener<Dimension>
		testListenerMin(AComponent::PROPERTY_MINIMUMSIZE, Dimension(50, 55), Dimension(230, 200));
	PropertyTestListener<Dimension>
		testListenerMax(AComponent::PROPERTY_MAXIMUMSIZE, Dimension(50, 55), Dimension(280, 210));
	comp->EventSender<PropertyChanged>::addEventListener(
			boost::bind(&PropertyTestListener<Dimension>::propertyChanged,
			&testListenerMin, _1, _2)
	);
	comp->EventSender<PropertyChanged>::addEventListener(
			boost::bind(&PropertyTestListener<Dimension>::propertyChanged,
			&testListenerMax, _1, _2)
	);
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< mini
	CPPUNIT_ASSERT(!comp->isMinimumSizeSet());
	CPPUNIT_ASSERT_EQUAL(Dimension(50, 55), comp->getMinimumSize());
	comp->setMinimumSize(Dimension(230, 200));
	CPPUNIT_ASSERT(comp->isMinimumSizeSet());
	CPPUNIT_ASSERT_EQUAL(Dimension(230, 200), comp->getMinimumSize());
	CPPUNIT_ASSERT_EQUAL(
			testListenerMin.createValidCopy(),
			testListenerMin
	);
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< maxi
	CPPUNIT_ASSERT(!comp->isMaximumSizeSet());
	CPPUNIT_ASSERT_EQUAL(Dimension(50, 55), comp->getMaximumSize());
	comp->setMaximumSize(Dimension(280, 210));
	CPPUNIT_ASSERT(comp->isMaximumSizeSet());
	CPPUNIT_ASSERT_EQUAL(Dimension(280, 210), comp->getMaximumSize());
	CPPUNIT_ASSERT_EQUAL(
		testListenerMax.createValidCopy(),
		testListenerMax
	);
}
//-----------------------------------------------------------------------------
void TestAComponent::test_name() {
	using namespace sambag::disco;
	using namespace sambag::com::events;
	using namespace sambag::disco::components;
	PropertyTestListener<std::string>
		testListener(AComponent::PROPERTY_NAME, "TestComponent", "xxx");
	comp->EventSender<PropertyChanged>::addEventListener(
			boost::bind(&PropertyTestListener<std::string>::propertyChanged,
			&testListener, _1, _2)
	);
	comp->setName("xxx");
	std::string str("[xxx,0,0,50x55,invalid]");
	CPPUNIT_ASSERT_EQUAL(std::string("xxx"), comp->getName());
	CPPUNIT_ASSERT_EQUAL(str, comp->toString());
}
//-----------------------------------------------------------------------------
void TestAComponent::test_preferredSize() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	PropertyTestListener<Dimension>
		testListener(AComponent::PROPERTY_PREFERREDSIZE,
				Dimension(50, 55), Dimension(230, 200));
	comp->EventSender<PropertyChanged>::addEventListener(
			boost::bind(&PropertyTestListener<Dimension>::propertyChanged,
			&testListener, _1, _2)
	);
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	CPPUNIT_ASSERT(!comp->isPreferredSizeSet());
	CPPUNIT_ASSERT_EQUAL(Dimension(50, 55), comp->getPreferredSize());
	comp->setPreferredSize(Dimension(230, 200));
	CPPUNIT_ASSERT(comp->isPreferredSizeSet());
	CPPUNIT_ASSERT_EQUAL(Dimension(230, 200), comp->getPreferredSize());
	CPPUNIT_ASSERT_EQUAL(
			testListener.createValidCopy(),
			testListener
	);
}
//-----------------------------------------------------------------------------
void TestAComponent::test_actionMap() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_autoscrolls() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_baseLine() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_border() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_grabFocus() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_getDrawContext() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_color() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_getTreeLock() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_ignoreRepaint() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_hasFocus() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_invalidate() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_isDisplayable() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_enabled() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_focusable() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_isFocusOwner() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_isOpaque() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_isShowing() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_isValid() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_visibility() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_requestFocus() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_revalidate() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_transferFocus() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_transferFocusBackward() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::test_validate() {
	testMissing();
}
//-----------------------------------------------------------------------------
void TestAComponent::testPutClientProperty() {
	using namespace sambag::com;
	using namespace sambag::disco;
	size_t origNum = comp->getClientProperties().size();
	comp->putClientPropertyImpl("index", createObject(101));
	ArbitraryType::Ptr v = comp->getClientProperty("index");
	CPPUNIT_ASSERT(v);
	int res = 0;
	get(v, res);
	CPPUNIT_ASSERT_EQUAL((int)101, res);
	// remove value
	comp->putClientPropertyImpl("index", ArbitraryType::Ptr());
	CPPUNIT_ASSERT_EQUAL(origNum, comp->getClientProperties().size());

	comp->putClientProperty("pos", Point2D(100, 101));
	Point2D p;
	comp->getClientProperty("pos", p);
	CPPUNIT_ASSERT_EQUAL(p, Point2D(100, 101));
}
//-----------------------------------------------------------------------------
void TestAComponent::testGetFirstAndLastParent() {
	using namespace sambag::com;
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	enum { NUM = 10 };
	AContainer::Ptr cont[NUM];
	for (int i=0; i<NUM; ++i) {
		if (i == NUM/2) {
			cont[i] = TestContainerVariant<0>::create();
		} else {
			cont[i] = TestContainer::create();
		}
		if (i==0)
			continue;
		cont[i-1]->add(cont[i]);
	}
	cont[NUM-1]->add(comp);
	
	comp->getFirstContainer<TestComponent>();
	CPPUNIT_ASSERT( comp->getFirstContainer<AContainer>() == cont[NUM-1] );
	CPPUNIT_ASSERT( comp->getLastContainer<AContainer>() == cont[0] );


	TestContainer::Ptr obj = comp->getFirstContainer<TestContainer>();
	CPPUNIT_ASSERT_EQUAL((void*)cont[NUM-1].get(), (void*)obj.get());
	
	obj = comp->getLastContainer<TestContainer>();
	CPPUNIT_ASSERT_EQUAL((void*)cont[0].get(), (void*)obj.get());

	TestContainerVariant<0>::Ptr obj2 = 
		comp->getLastContainer< TestContainerVariant<0> >();
	CPPUNIT_ASSERT_EQUAL((void*)cont[NUM/2].get(), (void*)obj2.get());

	CPPUNIT_ASSERT(
		!comp->getLastContainer< TestContainerVariant<1> >()
	);
}
} // namespace
