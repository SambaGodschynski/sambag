/*
 * TestGenFlowLayout.cpp
 *
 *  Created on: Sat Dec  8 14:37:13 2012
 *      Author: Johannes Unger
 */

#include "TestGenFlowLayout.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/genFormatter/GenFlowLayout.hpp>
#include <sambag/disco/genFormatter/RectangleAccess.hpp>
#include <sambag/disco/genFormatter/GenericFormatter.hpp>
#include <sambag/disco/Geometry.hpp>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestGenFlowLayout );

namespace tests {
//=============================================================================
//  Class TestGenFlowLayout
//=============================================================================
//-----------------------------------------------------------------------------
void TestGenFlowLayout::testRectangle() {
	using namespace sambag::disco;
	using namespace sambag::disco::genFormatter;
	
	typedef GenericFormatter< Rectangle,
		RectangleAccess,
		GenFlowLayout
	> Formatter;
	Formatter form;
	const size_t num = 10;	
	std::vector<Rectangle> rects;
	rects.reserve(num);
	for (int i=0; i<num; ++i) {
		Rectangle r(0 ,0 , (i+1)*10, (i+1)*15);
		rects.push_back(r);
		form.addComponent(&(rects.back()));	
	}
	CPPUNIT_ASSERT_EQUAL((size_t)num, form.getNumComponents());
	for (int i=0; i<num; ++i) {
		CPPUNIT_ASSERT(form.getComponent(i) == &(rects[i]));
	}
	
	// layout
	form.layout();	

	Rectangle exp01[] = { Rectangle(Point2D(0, 67.5), Point2D(10, 82.5)),
						Rectangle(Point2D(10, 60), Point2D(30, 90)),
						Rectangle(Point2D(30, 52.5), Point2D(60, 97.5)),
						Rectangle(Point2D(60, 45), Point2D(100, 105)),
						Rectangle(Point2D(100, 37.5), Point2D(150, 112.5)),
						Rectangle(Point2D(150, 30), Point2D(210, 120)),
						Rectangle(Point2D(210, 22.5), Point2D(280, 127.5)),
						Rectangle(Point2D(280, 15), Point2D(360, 135)),
						Rectangle(Point2D(360, 7.5), Point2D(450, 142.5)),
						Rectangle(Point2D(450, 0), Point2D(550, 150)) };
	for (int i=0; i<num; ++i) {
		CPPUNIT_ASSERT_EQUAL( exp01[i], rects[i] );
	}
	// change size
	form.setX(10.);
	form.setY(10.);
	form.setHgap(5.);
	form.setVgap(5.);
	form.setWidth(250.);
	form.setHeight(900.);
	form.setAlignment(Formatter::CENTER);
	form.layout();

	Rectangle exp02[] = {Rectangle(Point2D(12.5, 42.5), Point2D(22.5, 57.5)),
						Rectangle(Point2D(27.5, 35), Point2D(47.5, 65)),
						Rectangle(Point2D(52.5, 27.5), Point2D(82.5, 72.5)),
						Rectangle(Point2D(87.5, 20), Point2D(127.5, 80)),
						Rectangle(Point2D(132.5, 12.5), Point2D(182.5, 87.5)),
						Rectangle(Point2D(187.5, 5), Point2D(247.5, 95)),
						Rectangle(Point2D(5, 115), Point2D(75, 220)),
						Rectangle(Point2D(80, 107.5), Point2D(160, 227.5)),
						Rectangle(Point2D(165, 100), Point2D(255, 235)),
						Rectangle(Point2D(80, 240), Point2D(180, 390))};

	for (int i=0; i<num; ++i) {
		CPPUNIT_ASSERT_EQUAL( exp02[i], rects[i] );
	}

	// test remove
	form.removeComponent( &(rects[0]) );
	CPPUNIT_ASSERT_EQUAL((size_t)num-1, form.getNumComponents());
}
//-----------------------------------------------------------------------------
void TestGenFlowLayout::testBounds() {
	using namespace sambag::disco;
	using namespace sambag::disco::genFormatter;
	
	typedef GenericFormatter< Rectangle,
		RectangleAccess,
		GenFlowLayout
	> Formatter;
	Formatter form;
	const size_t num = 10;	
	std::vector<Rectangle> rects;
	rects.reserve(num);
	for (int i=0; i<num; ++i) {
		Rectangle r(0 ,0 , (i+1)*10, (i+1)*15);
		rects.push_back(r);
		form.addComponent(&(rects.back()));	
	}
	
	// layout
	form.layout();	
	
	Coordinate x, y, w, h;
	form.currentBounds(x,y,w,h);
	Rectangle rIst(x, y, w, h);
	Rectangle rSoll(0, 0, 0, 0);
	CPPUNIT_ASSERT_EQUAL(rSoll, rIst);
}
} //namespace
