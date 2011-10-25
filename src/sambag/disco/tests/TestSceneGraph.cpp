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
	std::string soll = "Draw(Image[i1.img]), Draw(Image[i2.img]), DrawAndRestoreContextState"
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

} // namespace
