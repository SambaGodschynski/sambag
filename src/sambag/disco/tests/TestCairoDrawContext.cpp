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

using namespace sambag::com;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestCairoDrawContext );

// TODO: create shared_ptr wrapper for surface
void releaseSurface( cairo_surface_t *surface ) {
	cairo_surface_destroy(surface);
}

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
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 320, 200);
	CairoDrawContext::Ptr context = CairoDrawContext::create( surface );
	cairo_surface_write_to_png(surface, (OUTPUT_FOLDER + "testConstructor.png").c_str() );
	context.reset();
	cairo_surface_destroy(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>check file exists
	CPPUNIT_ASSERT( filesystem::exists(OUTPUT_FOLDER + "testConstructor.png") );
	filesystem::remove(OUTPUT_FOLDER + "/testConstructor.png");

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testLine() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testArc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testRectangle() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testCurve() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testText() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testPath() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testClip() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testTransformation() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
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
	context.reset();
	releaseSurface(surface);

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
	CairoDrawContext::discoMatrixToCairoMatrix(m, cm);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>and back
	Matrix dm(3,3);
	CairoDrawContext::cairoMatrixToDiscoMatrix(cm, dm);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>compare
	CPPUNIT_ASSERT( m == dm );
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testLineStyle() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> prepare dash
	IDrawContext::Dash dash;
	{
		Number dashes[] = {5.0, 5.0, 10.0, 5.0};
		int numDash = sizeof(dashes)/sizeof(dashes[0]);
		Number offset = 0.0;
		dash = IDrawContext::Dash(dashes, numDash, offset);
	}
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

	{
		Number dashes[] = { 50.0, 50.0 };
		int numDash = sizeof(dashes)/sizeof(dashes[0]);
		Number offset = 0.0;
		dash = IDrawContext::Dash(dashes, numDash, offset);
	}

	context->setDash(dash);
	context->setStrokeWidth(50.0);
	context->moveTo( Point2D(900, 300)); context->lineTo( Point2D(1100, 100) );
	context->stroke();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>write and test
	testPng("testLineStyle", surface);
	context.reset();
	releaseSurface(surface);

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testSetStrokeWidth() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set/compare
	context->setStrokeWidth(5.0);
	CPPUNIT_ASSERT_EQUAL ( 5.0, context->getStrokeWidth() );
	//
	context.reset();
	releaseSurface(surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testSetColor() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	std::stringstream ss;
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set/compare
	context->setFillColor( ColorRGBA( 0.5, 0.25, 0.125 ) );
	CPPUNIT_ASSERT ( ColorRGBA( 0.5, 0.25, 0.125 ) == context->getFillColor() );
	context->setStrokeColor( ColorRGBA( 1.5, 0.25, 0.125 ) );
	CPPUNIT_ASSERT ( ColorRGBA( 1.5, 0.25, 0.125 ) == context->getStrokeColor() );

	//
	context.reset();
	releaseSurface(surface);
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testMisc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	std::stringstream ss;
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>has current point / get current point
	CPPUNIT_ASSERT( !context->hasCurrentPoint() );
	Point2D p0(1., .5);
	context->moveTo( p0 );
	CPPUNIT_ASSERT( context->hasCurrentPoint() );
	//CPPUNIT_ASSERT( equals(p0, context->getCurrentPoint()) );
	//
	context.reset();
	releaseSurface(surface);
}


} //namespaces
