/*
 * TestSceneGraph.cpp
 *
 *  Created on: 29.10.2011
 *      Author: samba
 */

#include <cppunit/config/SourcePrefix.h>
#include "TestSceneGraph.hpp"
#include <string>
#include <list>
#include <sstream>
#include <vector>
#include "sambag/disco/graphicElements/SceneGraph.hpp"
#include "sambag/disco/graphicElements/Image.hpp"
#include "sambag/com/Common.hpp"
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestSceneGraph );

namespace tests {
//=============================================================================
// TestSceneGraph
//=============================================================================
//-----------------------------------------------------------------------------
void TestSceneGraph::testConstructor() {
	using namespace sambag::disco;
	graphicElements::SceneGraph::Ptr g = graphicElements::SceneGraph::create();
	CPPUNIT_ASSERT(g);
	CPPUNIT_ASSERT_EQUAL(std::string("{}"), g->processListAsString());
}
//-----------------------------------------------------------------------------
/*
 *			i1
 *			/ \
 *		  i2   i3
 *		 /  \
 *		i4  i5
 */
void TestSceneGraph::testBuildGraph() {
	using namespace sambag::disco::graphicElements;
	SceneGraph::Ptr g = SceneGraph::create();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<build elements
	Image::Ptr i1 = Image::create();
	i1->setUri("i1.img");
	Image::Ptr i2 = Image::create();
	i2->setUri("i2.img");
	Image::Ptr i3 = Image::create();
	i3->setUri("i3.img");
	Image::Ptr i4 = Image::create();
	i4->setUri("i4.img");
	Image::Ptr i5 = Image::create();
	i5->setUri("i5.img");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<add and connect
	g->addElement(i1); g->addElement(i2); g->addElement(i3);
	g->addElement(i4); g->addElement(i5);
	g->connectElements(i1, i2);
	g->connectElements(i1, i3);
	g->connectElements(i2, i4);
	g->connectElements(i2, i5);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<compare
	std::string soll = "{Draw(Image[i1.img]), Draw(Image[i2.img]), DrawAndRestoreContextState"
			"(Image[i4.img]), DrawAndRestoreContextState(Image[i5.img]), RestoreContextState"
			", DrawAndRestoreContextState(Image[i3.img]), RestoreContextState}";
	CPPUNIT_ASSERT_EQUAL(soll, g->processListAsString());
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> check scenegraph vertex order
	typedef SceneGraph::OrderNumber OrderNumber;
	SceneGraph::G bg = g->getGraphImpl();
	SceneGraph::VertexIterator vi, end;
	boost::tie(vi, end) = boost::vertices(bg);
	std::vector<OrderNumber> res;
	res.reserve( boost::num_vertices(bg) );
	for (; vi!=end; ++vi) {
		res.push_back( g->getVertexOrderNumber(*vi) );
	}
	OrderNumber sollO[] = {-1,0,1,0,1};
	const size_t N_SOLL = sizeof(sollO) / sizeof(sollO[0]);
	CPPUNIT_ASSERT_EQUAL(N_SOLL, res.size());
	bool eq = std::equal(res.begin(), res.end(), &sollO[0]);
	if (!eq) {
		std::stringstream ss1;
		ss1<<"{";
		std::stringstream ss2;
		size_t i = 0;
		for_each(OrderNumber on, res) {
			ss1<<on<<", ";
			ss2<<sollO[i++]<<", ";
		}
		ss1<<"} is not "<< std::endl <<"{"<<ss2.str()<<"}";
		CPPUNIT_ASSERT_MESSAGE(ss1.str(), false);
	}
}
//-----------------------------------------------------------------------------
void TestSceneGraph::testTransformNode() {
	using namespace sambag::disco::graphicElements;
	using namespace sambag::com;
	SceneGraph::Ptr g = SceneGraph::create();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<build elements
	Image::Ptr i1 = Image::create();
	i1->setUri("i1.img");
	Image::Ptr i2 = Image::create();
	i2->setUri("i2.img");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<add and connect
	g->addElement(i1); g->addElement(i2);
	g->connectElements(i1, i2);
	Matrix m(3, 3);
	m(0,0) = 0; m(0,1) = 0.1; m(0,2) = 0.2;
	m(1,0) = 1; m(1,1) = 1.1; m(1,2) = 1.2;
	m(2,0) = 2; m(2,1) = 2.1; m(2,2) = 2.2;
	CPPUNIT_ASSERT(g->setTransfomationTo(i1, m));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<compare
	CPPUNIT_ASSERT( g->getTransformationOf(i1) == m );
	CPPUNIT_ASSERT( g->getTransformationOf(i2) != m );
}
//-----------------------------------------------------------------------------
void TestSceneGraph::testStyleNode() {
	using namespace sambag::disco::graphicElements;
	using namespace sambag::com;
	SceneGraph::Ptr g = SceneGraph::create();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<build elements
	Image::Ptr i1 = Image::create();
	i1->setUri("i1.img");
	Image::Ptr i2 = Image::create();
	i2->setUri("i2.img");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<add and connect
	g->addElement(i1); g->addElement(i2);
	g->connectElements(i1, i2);
	Style s;
	s.strokeColor( ColorRGBA(1) );
	s.fillColor( ColorRGBA(0,1) );
	CPPUNIT_ASSERT(g->setStyleTo(i1, s));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<compare
	CPPUNIT_ASSERT( g->getStyleOf(i1) == s );
	CPPUNIT_ASSERT( g->getStyleOf(i2) != s );
}
//-----------------------------------------------------------------------------
void TestSceneGraph::testIdAndClassRelations() {
	using namespace sambag::disco::graphicElements;
	using namespace sambag::com;
	using namespace sambag::disco;
	SceneGraph::Ptr g = SceneGraph::create();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<build elements
	Image::Ptr i1 = Image::create();
	i1->setUri("i1.img");
	Image::Ptr i2 = Image::create();
	i2->setUri("i2.img");
	Image::Ptr i3 = Image::create();
	i2->setUri("i3.img");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<add and connect
	g->addElement(i1); g->addElement(i2);
	CPPUNIT_ASSERT( g->registerElementClass(i1, "classA") );
	CPPUNIT_ASSERT( g->registerElementClass(i2, "classA") );
	CPPUNIT_ASSERT( !g->registerElementClass(i2, "classA") ); // twice registered
	CPPUNIT_ASSERT( g->registerElementId(i2,"idA") );
	CPPUNIT_ASSERT( !g->registerElementId(i1, "idA") ); // idA already registerd
	CPPUNIT_ASSERT( !g->registerElementId(i3, "idA") ); // i3 not in graph
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<assert
	IDrawable::Ptr res = g->getElementById("qwerty");
	CPPUNIT_ASSERT(!res);
	res = g->getElementById("idA");
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT_EQUAL((void*)res.get(), (void*)i2.get()); // compare object identity
	std::list<IDrawable::Ptr> resList, resListCopy;
	g->getElementsByClass("classA", resList);
	CPPUNIT_ASSERT_EQUAL((size_t)2, resList.size());
	// we can't test the resList with compare because we don't now the
	// order of the elements in there.
	resListCopy = resList; // we wan to remove objects while iterating,
						   // so we have to copy resList.
	for_each(IDrawable::Ptr obj, resListCopy) {
		// remove accepted elements
		if (obj.get() == i1.get())
			resList.remove(obj);
		if (obj.get() == i2.get())
			resList.remove(obj);
	}
	CPPUNIT_ASSERT(resList.empty());
}
} // namespace
