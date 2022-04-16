/*
 * TestList.cpp
 *
 *  Created on: Wed Sep  5 13:53:00 2012
 *      Author: Johannes Unger
 */

#include "TestList.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/List.hpp>
#include <string>
#include <list>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestList );

namespace tests {
//=============================================================================
//  Class TestList
//=============================================================================
//-----------------------------------------------------------------------------
void TestList::testDefaultListModel() {
	using namespace sambag::disco::components;
	List<std::string>::Ptr model = List<std::string>::create();

	CPPUNIT_ASSERT_EQUAL((size_t)0, model->size());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<addingElements
	model->addElement(std::string("element01"));
	CPPUNIT_ASSERT_EQUAL((size_t)1, model->size());
	CPPUNIT_ASSERT_EQUAL(std::string("element01"), model->firstElement());
	CPPUNIT_ASSERT_EQUAL(std::string("element01"), model->lastElement());
	model->addElement(std::string("element02"));
	CPPUNIT_ASSERT_EQUAL((size_t)2, model->size());
	CPPUNIT_ASSERT_EQUAL(std::string("element01"), model->firstElement());
	CPPUNIT_ASSERT_EQUAL(std::string("element02"), model->lastElement());
	// void addElement(size_t index, const T &element)
	model->addElement(std::string("element00"), 0);
	CPPUNIT_ASSERT_EQUAL((size_t)3, model->size());
	CPPUNIT_ASSERT_EQUAL(std::string("element00"), model->firstElement());
	CPPUNIT_ASSERT_EQUAL(std::string("element02"), model->lastElement());
	// const T & get(size_t index) const
	CPPUNIT_ASSERT_EQUAL(std::string("element00"), model->get(0));
	CPPUNIT_ASSERT_EQUAL(std::string("element01"), model->get(1));
	CPPUNIT_ASSERT_EQUAL(std::string("element02"), model->get(2));
	// T set(size_t index, const T & element)
	CPPUNIT_ASSERT_EQUAL(std::string("element02"), model->set(2, std::string("c")));
	CPPUNIT_ASSERT_EQUAL(std::string("c"), model->get(2));
	// T removeElement(size_t index)
	CPPUNIT_ASSERT_EQUAL(std::string("element01"), model->removeElementAt(1));
	CPPUNIT_ASSERT_EQUAL(std::string("element00"), model->firstElement());
	CPPUNIT_ASSERT_EQUAL(std::string("c"), model->lastElement());
	// bool removeElement(const T &element)
	CPPUNIT_ASSERT_EQUAL((bool)true, model->removeElement("c"));
	CPPUNIT_ASSERT_EQUAL(std::string("element00"), model->firstElement());
	CPPUNIT_ASSERT_EQUAL(std::string("element00"), model->lastElement());
	// bool containsElement(const T &elem) const
	CPPUNIT_ASSERT_EQUAL((bool)true, model->containsElement("element00"));
	// int indexOf(const T &elem) const
	CPPUNIT_ASSERT_EQUAL((int)0, model->indexOf("element00"));
	CPPUNIT_ASSERT_EQUAL((int)-1, model->indexOf("grütze"));
	for (size_t i=0; i<100; ++i)
		model->addElement("xxx");
	model->addElement("yyy");
	// int indexOf(const T &elem, size_t index) const
	CPPUNIT_ASSERT_EQUAL((int)1, model->indexOf("xxx", 0));
	model->set(0, "xxx");
	CPPUNIT_ASSERT_EQUAL((int)0, model->indexOf("xxx"));
	CPPUNIT_ASSERT_EQUAL((int)1, model->indexOf("xxx", 0));
	CPPUNIT_ASSERT_EQUAL((int)2, model->indexOf("xxx", 1));
	CPPUNIT_ASSERT_EQUAL((int)100, model->indexOf("xxx", 99));
	CPPUNIT_ASSERT_EQUAL((int)-1, model->indexOf("xxx", 100));
	// int lastIndexOf(const T &elem)
	CPPUNIT_ASSERT_EQUAL((int)100, model->lastIndexOf("xxx"));
	CPPUNIT_ASSERT_EQUAL((int)100, model->lastIndexOf("xxx", 50));
	CPPUNIT_ASSERT_EQUAL((int)101, model->lastIndexOf("yyy", 100));
	CPPUNIT_ASSERT_EQUAL((int)-1, model->lastIndexOf("xxx", 101));
	// void removeRange(size_t fromIndex, size_t toIndex)
	model->removeRange(1, 100);
	CPPUNIT_ASSERT_EQUAL((size_t)2, model->size());
	CPPUNIT_ASSERT_EQUAL(std::string("xxx"), model->firstElement());
	CPPUNIT_ASSERT_EQUAL(std::string("yyy"), model->lastElement());
	//	void copyInto(STLContainer &cont)
	std::list<std::string> l;
	model->copyInto(l);
	CPPUNIT_ASSERT_EQUAL((size_t)2, l.size());
	CPPUNIT_ASSERT_EQUAL(std::string("xxx"), *(l.begin()));
	CPPUNIT_ASSERT_EQUAL(std::string("yyy"), *(--(l.end())));
	// void clear()
	model->clear();
	// bool isEmpty() const
	CPPUNIT_ASSERT_EQUAL((bool)true, model->isEmpty());
}
} //namespace
