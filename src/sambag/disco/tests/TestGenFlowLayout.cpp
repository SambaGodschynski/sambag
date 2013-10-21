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
#include <sstream>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestGenFlowLayout );

namespace {

	std::string toString( sambag::disco::Rectangle *r, size_t n) {
		std::stringstream ss;
		for (size_t i=0; i<n; ++i) {
			ss<<r[i]<<"; ";
		}
		return ss.str();
	}

}

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

	std::string exp01 = "Rectangle(Point2D(0, 67.5), Point2D(10, 82.5)); Rectangle(Point2D(1\
0, 60), Point2D(30, 90)); Rectangle(Point2D(30, 52.5), Point2D(60, 97.5)); Rect\
angle(Point2D(60, 45), Point2D(100, 105)); Rectangle(Point2D(100, 37.5), Point2\
D(150, 112.5)); Rectangle(Point2D(150, 30), Point2D(210, 120)); Rectangle(Point\
2D(210, 22.5), Point2D(280, 127.5)); Rectangle(Point2D(280, 15), Point2D(360, 1\
35)); Rectangle(Point2D(360, 7.5), Point2D(450, 142.5)); Rectangle(Point2D(450,\
 0), Point2D(550, 150)); ";
	
	CPPUNIT_ASSERT_EQUAL( exp01, toString(&rects[0], num) );

	// change size
	form.setX(10.);
	form.setY(10.);
	form.setHgap(5.);
	form.setVgap(5.);
	form.setWidth(250.);
	form.setHeight(900.);
	form.setAlignment(Formatter::CENTER);
	form.layout();

	std::string exp02 = "Rectangle(Point2D(22.5, 52.5), Point2D(32.5, 67.5)); Rectangle(Poin\
t2D(37.5, 45), Point2D(57.5, 75)); Rectangle(Point2D(62.5, 37.5), Point2D(92.5,\
 82.5)); Rectangle(Point2D(97.5, 30), Point2D(137.5, 90)); Rectangle(Point2D(14\
2.5, 22.5), Point2D(192.5, 97.5)); Rectangle(Point2D(197.5, 15), Point2D(257.5,\
 105)); Rectangle(Point2D(15, 125), Point2D(85, 230)); Rectangle(Point2D(90, 11\
7.5), Point2D(170, 237.5)); Rectangle(Point2D(175, 110), Point2D(265, 245)); Re\
ctangle(Point2D(90, 250), Point2D(190, 400)); ";

	CPPUNIT_ASSERT_EQUAL( exp02 , toString(&rects[0], num) );


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
	Rectangle rSoll(0, 0, 550, 150);
	CPPUNIT_ASSERT_EQUAL(rSoll, rIst);
}
} //namespace
