/**
 * TestCairoDrawContext.cpp
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
#include "sambag/com/Common.hpp"
#include <sstream>
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include "sambag/disco/svg/graphicElements/RefElement.hpp"
#include "sambag/disco/IDiscoFactory.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestSvg );

using namespace sambag::com;

static const std::string IN_FOLDER = "testimages/";
static const std::string DOC_TITLE = "D.I.S.C.O. testSvg::output";

void includeJs(tests::TestSuiteHtmlOutput::Ptr obj) {
	static const std::string URI = "js/";
	obj->doc().script().type("text/javascript").src(URI + "jquery.min.js").end();
	obj->doc().script().type("text/javascript").src(URI + "jquery-ui.min.js").end();
	obj->doc().script().type("text/javascript").src(URI + "jquery.beforeafter-1.4.js").end();
}

tests::TestSuiteHtmlOutput::Ptr openDoc() {
	tests::TestSuiteHtmlOutput::Ptr neu=
		tests::TestSuiteHtmlOutput::create("svgTestSuite.html");
	// prepare doc
	neu->doc().html().head().title().text(DOC_TITLE).end(2).body();
	includeJs(neu);
	neu->doc().link().rel("stylesheet").type("text/css").href("htmlOut.css").end();
	neu->doc().h1().text(DOC_TITLE).end();
	neu->doc().p().id("timestamp");
	neu->doc().text("These tests ran on " + tests::getTimeStampAsStr() + ":").end();
	// prepare index
	neu->index().a().name("index");
	neu->index().table();
	// prepare content
	neu->content().h1().text("Input/Output in detail:").end();
	return neu;
}


tests::TestSuiteHtmlOutput::Ptr html = openDoc();

namespace tests {
//=============================================================================
// TestSvg::setUp
//=============================================================================
//-----------------------------------------------------------------------------
TestSvg::TestSvg() {
	setupEnv();
}
//-----------------------------------------------------------------------------
TestSvg::~TestSvg() {
}
//-----------------------------------------------------------------------------
void TestSvg::setUp() {
}
//-----------------------------------------------------------------------------
void TestSvg::tearDown() {
}
//-----------------------------------------------------------------------------
void TestSvg::testStyleAdd() {
	using namespace sambag::disco;
	using namespace sambag::disco::svg::graphicElements;
	Style nullStyle;
	Style values;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	values.fillColor(ColorRGBA(0));
	values.strokeColor(ColorRGBA(1));
	values.strokeWidth(10);
	std::vector<Number> v; v.push_back(1);
	values.dash(Dash::createWithValues(v));
	values.font(sambag::disco::Font());
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	Style n01;
	n01.add(values);
	CPPUNIT_ASSERT(n01==values);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	n01.add(nullStyle);
	CPPUNIT_ASSERT(n01==values);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgFirstElements() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "firstElements.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//CPPUNIT_ASSERT_EQUAL(12.*35.43307, (Number)rootObject->getSize().getWidth());
	//CPPUNIT_ASSERT_EQUAL(4.*35.43307, (Number)rootObject->getSize().getHeight());
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> manipulate object
	IDrawable::Ptr text = g->getElementById("#text");
	CPPUNIT_ASSERT(text);
	svg::graphicElements::SceneGraph::StylePtr textStyle = g->getStyleRef(text);
	textStyle->fillColor(ColorRGBA(0,1,0));
	g->draw(context);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	testSvg("testSvgFirstElements",TEST_SVG, surface, html,
		"the green text is only in the output image.");
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform01() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform01.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 120);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgTransform01", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform02() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform02.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 120);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgTransform02", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform03() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform03.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 120);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgTransform03", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform04() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform04.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 150);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgTransform04", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgTransform05() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "transform05.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 150);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgTransform05", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path01.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	g->draw(context); // draw 2 times: drawInstructions, drawPath
	testSvg("testSvgPath01", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath02() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path02.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath02", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath03() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path03.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 525);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath03", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath04() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path04.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(500, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath04", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath04b() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path04b.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(500, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath04b", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath05() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path05.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 600);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath05", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath05b() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path05b.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 600);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath05b", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPath06() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "path06.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1000, 1000);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPath06", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPathValues() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "pathValues.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testPathValues", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPolyline() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "polyline.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPolyline", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgPolygon() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "polygon.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgPolygon", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgStdFill() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "stdfill.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(400, 120);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgStdFill", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgArc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "arc.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgArc", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgUse() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "use.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 525);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgUse", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgFont() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "font.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgFont", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgOpacity() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "opacity.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 350);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgOpacity", TEST_SVG,  surface, html,
		"can't draw correctly the second circle pair on the bottom left."
		" Chromium can't handle the opacity value of the upper right cirlce.");
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgImage() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "image.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgImage", TEST_SVG,  surface, html,
			"svg root needs path data to find image file.");
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgStyle() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "style.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgStyle", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgStyle2() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "style2.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 500);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgStyle2", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testLineCapStyle() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "linecap.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 200);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgLinecapStyle", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testLineJoinStyle() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "linejoin.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 350);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgLinejoinStyle", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testSvgGradient() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "gradient01.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(800, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testSvgGradient01", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testBoundingBoxes() {
	using namespace sambag::disco;
	using namespace boost::assign;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "boundingBoxes.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 900);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw boxes
	// get all of class=="bBox"
	std::list<IDrawable::Ptr> l;
	g->getElementsByClass(".bBox",l);
	CPPUNIT_ASSERT_MESSAGE("no elements with '.bBox' class found.",!l.empty());
	// prepare context
	context->identityMatrix();
	context->setStrokeColor(ColorRGBA(1,0,0,0.6));
	std::vector<Number> dashes;
	dashes+=5., 5.;
	context->setDash(Dash::createWithValues(dashes));
	context->setStrokeWidth(2);
	// draw
	boost_for_each(IDrawable::Ptr obj, l) {
		//if (!boost::dynamic_pointer_cast<graphicElements::RefElement>(obj))
		//	continue;
		context->rect(g->getBoundingBox(obj, context));
		context->stroke();
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	testSvg("testSvgBoundingBoxes", TEST_SVG,  surface, html);
}
//-----------------------------------------------------------------------------
void TestSvg::testIssue146() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "svglogo.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(500, 500);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testIssue146",
			TEST_SVG,
			surface,
			html,
			"see <a href='http://beta.vstforx.de/view.php?id=146'>issue#146</a>"
	);
}
//-----------------------------------------------------------------------------
void TestSvg::testUnits() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assume test file
	static const std::string TEST_SVG = IN_FOLDER + "testUnits.svg";
	CPPUNIT_ASSERT(boost::filesystem::exists(TEST_SVG));
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>build svg
	svg::SvgBuilder builder;
	svg::SvgRoot::Ptr rootObject = boost::dynamic_pointer_cast<svg::SvgRoot, svg::SvgObject>
			( builder.buildSvgFromFilename(TEST_SVG) );
	CPPUNIT_ASSERT(rootObject);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> create png
	IDiscoFactory *fac = getDiscoFactory();
	IImageSurface::Ptr surface = fac->createImageSurface(1200, 400);
	IDrawContext::Ptr context = fac->createContext(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	svg::graphicElements::SceneGraph::Ptr g = rootObject->getRelatedSceneGraph();
	g->draw(context);
	testSvg("testUnits", TEST_SVG, surface, html);
}
} //namespaces
