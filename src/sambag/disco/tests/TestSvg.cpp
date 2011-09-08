/**
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
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "TestSvg.hpp"
#include "sambag/disco/svg/SvgLine.hpp"
#include <cppunit/config/SourcePrefix.h>
#include "HelperForTesting.hpp"
#include <string>
#include <boost/filesystem.hpp>
#include "sambag/disco/svg/SvgBuilder.hpp"
#include "sambag/disco/svg/SvgRoot.hpp"
#include <list>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestSvg );

using namespace sambag::com;

static const std::string IN_FOLDER = "testimages/";


namespace tests {
//=============================================================================
// TestSvg::setUp
//=============================================================================
void TestSvg::setUp() {
	setupEnv();
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgFirstElements() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "firstElements.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	testPng("testSvgFirstElements", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> some asserts
	typedef std::list<svg::SvgObject::Ptr> SvgObjects;
	SvgObjects c;
	rootObject->getObjectsByClass<SvgObjects>("line",c);
	CPPUNIT_ASSERT(!c.empty());
	svg::SvgObject::Ptr text = rootObject->getObjectById("text");
	CPPUNIT_ASSERT(text);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform01() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform01.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 120);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	testPng("testSvgTransform01", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform02() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform02.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 120);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	testPng("testSvgTransform02", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform03() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform03.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 120);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	testPng("testSvgTransform03", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform04() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform04.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 150);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	testPng("testSvgTransform04", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path01.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	rootObject->draw(context); // draw 2 times: drawInstructions, drawPath
	testPng("testSvgPath01", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath02() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path02.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	rootObject->draw(context); // draw 2 times: drawInstructions, drawPath
	testPng("testSvgPath02", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath03() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path03.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 525);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	rootObject->draw(context); // draw 2 times: drawInstructions, drawPath
	testPng("testSvgPath03", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgStdFill() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "stdfill.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 120);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	rootObject->draw(context); // draw 2 times: drawInstructions, drawPath
	testPng("testSvgStdFill", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgArc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "arc.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1200, 400);
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	rootObject->draw(context);
	rootObject->draw(context); // draw 2 times: drawInstructions, drawPath
	testPng("testSvgArc", surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> release
	context.reset();
	cairo_surface_destroy(surface);
}
} //namespaces
