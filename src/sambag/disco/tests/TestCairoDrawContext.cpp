/*
 * TestCairoDrawContext.cpp
 *
 *
 *                                            Test Suite for CairoDraw Context
 *
 *                This test creates png files. For comparing it uses perceptualdiff (http://pdiff.sourceforge.net/)
 *                The Comparing is initiated via a system call to this script: outputfolder/comparepng.script
 *                Edit this script to adjust perecputaldiff. Or use another program to compare.
 *                Use "#define DISCO_CONTEXT_MAKE_REFERENCES" if you want create compare reference files.
 *
 *
 *                !Perceptualdiff converts png transparency to black, so use a background color to compare correctly!
 *
 *
 *
 *
 *
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#include "TestCairoDrawContext.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <boost/filesystem.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <sambag/com/Common.hpp>
#include "HelperForTesting.hpp"
#include <boost/assign/std/vector.hpp>
#include "sambag/disco/IDiscoFactory.hpp"
using namespace sambag::com;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestCairoDrawContext );

namespace tests {
//=============================================================================
// TestCairoDrawContext::setUp
//=============================================================================
void TestCairoDrawContext::setUp() {
	setupEnv();
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testConstructor() {
	using namespace sambag::disco;
	using namespace boost;
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surf = fac->createImageSurface(300, 200);
	IDrawContext::Ptr context = fac->createContext(surf);
	surf->writeToFile(OUTPUT_FOLDER + "testConstructor.png");
	context.reset();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>check file exists
	CPPUNIT_ASSERT( filesystem::exists(OUTPUT_FOLDER + "testConstructor.png") );
	filesystem::remove(OUTPUT_FOLDER + "/testConstructor.png");

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testLine() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw

	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	context->setStrokeColor( ColorRGBA(0) );

	context->setStrokeWidth(5.0);
	context->moveTo( Point2D(100, 300) ); context->lineTo( Point2D(300, 100) );
	context->stroke();

	context->setStrokeWidth(10.0);
	context->moveTo( Point2D(300, 300)); context->lineTo( Point2D(500, 100) );
	context->stroke();

	context->setStrokeWidth(15.0);
	context->moveTo( Point2D(500, 300)); context->lineTo( Point2D(700, 100) );
	context->stroke();

	context->setStrokeWidth(20.0);
	context->moveTo( Point2D(700, 300)); context->lineTo( Point2D(900, 100) );
	context->stroke();

	context->setStrokeWidth(50.0);
	context->moveTo( Point2D(900, 300)); context->lineTo( Point2D(1100, 100) );
	context->stroke();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testLine", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testArc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	context->setStrokeColor( ColorRGBA(0) );

	context->setStrokeWidth(10);
	context->setStrokeColor( ColorRGBA( 1.0, 0, 0 ) );
	context->arc( Point2D(600, 200), 100 );
	context->stroke();

	context->setFillColor( ColorRGBA( 0, 0, 1.0 ) );
	context->arc( Point2D(600, 200), 100 );
	context->fill();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testArc", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testRectangle() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();


	context->setStrokeWidth(10);
	context->setStrokeColor( ColorRGBA( 0, 0, 1.0 ) );
	context->rect( Rectangle(Point2D(400,100), 400, 200 ) );
	context->stroke();

	context->setFillColor( ColorRGBA( 1.0, 1.0, 0.0 ) );
	context->rect( Rectangle(Point2D(400,100), 400, 200 ) );
	context->fill();

	context->setStrokeColor( ColorRGBA( 1.0, 0.0, 0.0 ) );
	context->rect(Rectangle(500, 150, 400, 200), 15.0 );
	context->stroke();

	context->setFillColor( ColorRGBA( 0.0, 1.0, 0.3 ) );
	context->rect(Rectangle(500, 150, 400, 200), 15.0 );
	context->fill();

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testRectangle", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testCurve() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	context->setStrokeColor( ColorRGBA(0) );

	context->setStrokeWidth(10);
	context->curveTo( Point2D(100,200), Point2D(100,100), Point2D(250,100) );
	context->stroke();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testCurve", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testText() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	context->setStrokeColor( ColorRGBA(0) );

	Font font;
	font.size = 184.;
	font.fontFace = "arial";
	context->setFont(font);
	context->setFillColor( ColorRGBA(1.0) );
	context->moveTo( Point2D(150., 250.) );
	context->textPath("D.I.S.C.O");
	context->fill();
	context->setStrokeColor( ColorRGBA(0.0) );
	context->moveTo( Point2D(150., 250.) );
	context->textPath("D.I.S.C.O");
	context->stroke();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testText", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testPath() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();

	Font font;
	font.size = 184.;
	font.fontFace = "arial";
	context->setFont(font);
	context->setStrokeColor( ColorRGBA(1.0) );
	context->translate( Point2D(150., 200.) );
	context->textPath("D.I.S.C.O");
	Path::Ptr path = context->copyPath();
	context->stroke();
	context->setFillColor( ColorRGBA(1.0, 1.0) );
	context->appendPath(path);
	context->fill();
	Number rot=0.0;
	for ( Number a=1.0; a>0; a-=0.025) {
		context->setStrokeColor( ColorRGBA(1.0, 0, 0, a) );
		context->appendPath(path);
		context->stroke();
		context->setFillColor( ColorRGBA(1.0, 1.0, 0, a) );
		context->appendPath(path);
		context->fill();
		context->rotate(rot);
		rot+=0.0001;
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testPath", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testClip() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	context->setStrokeColor( ColorRGBA(0) );

	context->rect( Rectangle( Point2D(150., 120.), 105., 15.) );
	context->stroke();
	context->rect( Rectangle( Point2D(150., 120.), 105., 15.) );
	context->clip();
	Font font;
	font.size = 34.;
	font.fontFace = "arial";
	context->setFont(font);
	context->setFillColor( ColorRGBA(1.0) );
	context->moveTo( Point2D(150., 150.) );
	context->textPath("DISCO");
	context->fill();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testClip", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testTransformation() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> prepare
	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set tr matrix
	Matrix m(3,3);
	m(0,0) = 1;  m(0,1) = 0.5; m(0,2) = 0;
	m(1,0) = 0;  m(1,1) = 1; m(1,2) = 1;
	m(2,0) = 0;  m(2,1) = 0; m(2,2) = 0;
	context->scale( Point2D(1.2, 2.5) );
	context->translate( Point2D(100., 135.) );
	context->transform(m);
	context->rotate(.02);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	Font font;
	font.size = 184.;
	font.fontFace = "arial";
	context->setFont(font);
	context->setFillColor( ColorRGBA(1.0) );
	context->moveTo( Point2D(0., 0.) );
	context->textPath("D.I.S.C.O");
	context->fill();
	context->setStrokeColor( ColorRGBA(0.0) );
	context->moveTo( Point2D(0, 0) );
	context->textPath("D.I.S.C.O");
	context->stroke();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testTransformation", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testMatrixConv() {
	using namespace sambag::disco;
	using namespace boost::numeric::ublas;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>init matrix
	Matrix m(3,3);
	m(0,0) = 1;  m(0,1) = 0.5; m(0,2) = 0.21;
	m(1,0) = 2;  m(1,1) = 1.9; m(1,2) = 1.11;
	m(2,0) = 0;  m(2,1) = 0; m(2,2) = 0; // unused by cairo matrix
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>to cairo matrix
	cairo_matrix_t cm;
	discoMatrixToCairoMatrix(m, cm);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>and back
	Matrix dm(3,3);
	cairoMatrixToDiscoMatrix(cm, dm);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>compare
	CPPUNIT_ASSERT( m == dm );
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testDash() {
	using namespace sambag::disco;
	using namespace boost::assign;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>test creation
	std::vector<Number> dashes;
	dashes += 5.0, 5.0;
	Dash::Ptr dash = Dash::create(dashes);
	CPPUNIT_ASSERT_EQUAL((size_t)2, dash->size());
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>test comparison
	Dash::Ptr dash2 = Dash::create(dashes);
	CPPUNIT_ASSERT_EQUAL((size_t)2, dash2->size());
	CPPUNIT_ASSERT(*(dash.get()) == *(dash2.get()));
	dash2->offset(2.);
	CPPUNIT_ASSERT(*(dash.get()) != *(dash2.get()));
	dash2->offset(0);
	CPPUNIT_ASSERT(*(dash.get()) == *(dash2.get()));
	dash2->values()[0] = 0.;
	CPPUNIT_ASSERT(*(dash.get()) != *(dash2.get()));
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testLineStyle() {
	using namespace sambag::disco;
	using namespace boost::assign;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> prepare dash
	std::vector<Number> dashes;
	dashes += 5.0, 5.0, 10.0, 5.0;
	Dash::Ptr dash = Dash::create(dashes);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw

	context->setFillColor( ColorRGBA(1.0, 1.0, 1.0) );
	context->rect( Rectangle(0.,0.,1200.,400.) );
	context->fill();
	context->setStrokeColor( ColorRGBA(0) );

	context->setStrokeWidth(5.0);
	context->setDash(dash);
	context->moveTo( Point2D(100, 300) ); context->lineTo( Point2D(300, 100) );
	context->stroke();
	context->disableDash();

	CairoDrawContext::Ptr cr = boost::shared_dynamic_cast<CairoDrawContext>(context);

	// ensure that cairo_set_dash make deep copies.
	Number *ddash = new Number[2];
	ddash[0] = 10;
	ddash[1] = 10;
	cairo_set_dash(cr->getCairoContext(), ddash, 2, 0.);
	delete[] ddash;

	context->setLineCap( IDrawContext::LINE_CAP_SQUARE );
	context->setStrokeWidth(10.0);
	context->moveTo( Point2D(300, 300)); context->lineTo( Point2D(500, 100) );
	context->stroke();

	context->setStrokeWidth(15.0);
	context->setLineCap( IDrawContext::LINE_CAP_ROUND );
	context->moveTo( Point2D(500, 300)); context->lineTo( Point2D(700, 100) );
	context->stroke();



	context->setStrokeWidth(20.0);
	context->setLineCap( IDrawContext::LINE_CAP_BUTT );
	context->setDash(dash);
	context->moveTo( Point2D(700, 300)); context->lineTo( Point2D(900, 100) );
	context->stroke();

	dashes.clear();
	dashes += 50.0, 50.0;
	dash = Dash::create(dashes);
	context->setDash(dash);
	context->setStrokeWidth(50.0);
	context->moveTo( Point2D(900, 300)); context->lineTo( Point2D(1100, 100) );
	context->stroke();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testLineStyle", surface);

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testSetStrokeWidth() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set/compare
	context->setStrokeWidth(5.0);
	CPPUNIT_ASSERT_EQUAL ( (double)5.0, (double)context->getStrokeWidth() );
}
//-----------------------------------------------------------------------------
void drawGradients(sambag::disco::IDrawContext::Ptr context,
	const sambag::math::Matrix trans = sambag::math::IDENTITY_MATRIX)
{
	using namespace sambag::disco;
	context->save();
	// linear gradient
	context->rect(Rectangle(0,0,100,50));
	context->setStrokeColor(ColorRGBA());
	context->stroke();
	context->rect(Rectangle(0,0,100,50));
	IDiscoFactory::Ptr fac = getDiscoFactory();
	ALinearPattern::Ptr sol = fac->createLinearPattern(Point2D(0,0), Point2D(100,0));
	sol->addColorStop(ColorRGBA(1,0,0), 0);
	sol->addColorStop(ColorRGBA(0,1,0), .5);
	sol->addColorStop(ColorRGBA(0,0,1), 1.0);
	sol->setMatrix(trans);
	context->setFillPattern(sol);
	context->fill();
	// radial gradinet
	context->translate(Point2D(130, 100));
	context->arc(Point2D(), 50, 0, 2 * M_PI);
	context->stroke();
	context->arc(Point2D(), 50, 0, 2 * M_PI);
	ARadialPattern::Ptr rad = fac->createRadialPattern(Point2D(0,0), 0, Point2D(0,0), 45);
	rad->addColorStop(ColorRGBA(1,0,0), 0);
	rad->addColorStop(ColorRGBA(0,1,0), .5);
	rad->addColorStop(ColorRGBA(0,0,1), 1.0);
	rad->setMatrix(trans);
	context->setFillPattern(rad);
	context->fill();
	context->restore();
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testGradient() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>draw
	// fill bk
	context->rect(Rectangle(0,0,1200,400));
	context->setFillPattern(fac->createSolidPattern(ColorRGBA(1,1,1)));
	context->fill();
	drawGradients(context);
	context->translate(Point2D(200,10));
	context->scale(Point2D(2.0, 2.0));
	drawGradients(context, scale2D(1., 2.));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testPattern", surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testSetColor() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set/compare
	context->setFillColor( ColorRGBA( 0.5, 0.25, 0.125 ) );
	CPPUNIT_ASSERT ( ColorRGBA( 0.5, 0.25, 0.125 ) == context->getFillColor() );
	context->setStrokeColor( ColorRGBA( 1.0, 0.25, 0.125 ) );
	CPPUNIT_ASSERT ( ColorRGBA( 1.0, 0.25, 0.125 ) == context->getStrokeColor() );
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testMisc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory::Ptr fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>has current point / get current point
	CPPUNIT_ASSERT( !context->hasCurrentPoint() );
	Point2D p0(1., .5);
	context->moveTo( p0 );
	CPPUNIT_ASSERT( context->hasCurrentPoint() );
	//CPPUNIT_ASSERT( equals(p0, context->getCurrentPoint()) );
}


} //namespaces
