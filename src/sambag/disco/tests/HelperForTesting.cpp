/*
 * TestHelper.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "HelperForTesting.hpp"
#include <boost/filesystem.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <sambag/com/Common.hpp>
#include <cairo.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tests {
//=============================================================================
// class TestSuiteHtmlOutput
//=============================================================================
//-----------------------------------------------------------------------------
TestSuiteHtmlOutput::TestSuiteHtmlOutput(const std::string &outfile) :
outfile(outfile)
{

	_doc = new sambag::xml::cpAtl::Html(sDoc);
	_index = new sambag::xml::cpAtl::Html(sIndex);
	_content = new sambag::xml::cpAtl::Html(sContent);
}
TestSuiteHtmlOutput::Ptr TestSuiteHtmlOutput::create(const std::string &outfile){
	return Ptr(new TestSuiteHtmlOutput(outfile));
}
sambag::xml::cpAtl::Html & TestSuiteHtmlOutput::doc() {
	return *_doc;
}
sambag::xml::cpAtl::Html & TestSuiteHtmlOutput::index() {
	return *_index;
}
sambag::xml::cpAtl::Html & TestSuiteHtmlOutput::content() {
	return *_content;
}
TestSuiteHtmlOutput::~TestSuiteHtmlOutput() {
	_index->endAll();
	_content->endAll();
	std::fstream outStream;
	outStream.open(outfile.c_str(), std::ios_base::out);
	outStream<<sDoc.str();
	outStream<<sIndex.str()<<sContent.str();
	sDoc.str(""); sDoc.flush();
	_doc->endAll();
	outStream<<sDoc.str();
	delete _doc;
	delete _index;
	delete _content;
	outStream.close();
}


const std::string OUTPUT_FOLDER = "output/";
const std::string REFERENCE_FOLDER = "references/";
const std::string COMPARE_SCRIPT = "sh " + OUTPUT_FOLDER + "comparepng.script ";

// make reference pictures for compare
#define DISCO_CONTEXT_MAKE_REFERENCES
//-----------------------------------------------------------------------------
bool comparePng( const std::string &f1, const std::string &f2) {
	std::string cmd(COMPARE_SCRIPT + f1 + " " + f2 );
	int r = system(cmd.c_str());
	return r == 0;
}
//-----------------------------------------------------------------------------
void writePng(sambag::disco::IImageSurface::Ptr surface, const std::string name)
{
	surface->writeToFile(name);
}
//-----------------------------------------------------------------------------
void testPng( const std::string &testName,
		sambag::disco::IImageSurface::Ptr surface )
{
	const std::string f1( OUTPUT_FOLDER + REFERENCE_FOLDER + testName + ".png" );
	const std::string f2( OUTPUT_FOLDER + testName + ".png" );

#ifdef DISCO_CONTEXT_MAKE_REFERENCES
	writePng(surface, f1);
#else
	writePng(surface, f2);
	CPPUNIT_ASSERT ( comparePng(f1, f2) );
#endif
}
//-----------------------------------------------------------------------------
void testSvg( const std::string &testName,
		const std::string &inSvg,
		sambag::disco::IImageSurface::Ptr surface,
		TestSuiteHtmlOutput::Ptr html,
		const std::string &comment )
{
	using namespace sambag::disco;
	// write file
	const std::string f1( OUTPUT_FOLDER + testName + ".png" );
	writePng(surface, f1);
	// index
	html->index().tr();
	html->index().td().a().href("#"+testName).text(testName).end(2);
	// preview image:
	Number dstW = 100.;
	if (surface->getSize().getWidth() == 0. )
		return;
	Number scal = dstW / surface->getSize().getWidth();
	std::string w = Coordinate(dstW).toString();
	std::string h = Coordinate(surface->getSize().getHeight() * scal).toString();
	html->index().td().img().src(f1).width(w).height(h).end(2);
	html->index().end();
	// content
	// input
	html->content().div().classAttr("testCase");
	html->content().h3().a().name(testName);
	html->content().text(testName).end(2);
	html->content().p().classAttr("imageDescr").text(inSvg + " // ");
	html->content().text(f1).end();
	// before/after
	std::string id = "before-after-" + testName;
	html->content().div().id(id);
	// svg
	w = surface->getSize().getWidth().toString();
	h = surface->getSize().getHeight().toString();
	html->content().div();
	html->content().object().data(inSvg).type("image/svg+xml");
	html->content().width(w).height(h).param().name("src").value(inSvg).end(2);
	html->content().end();
	// img
	html->content().div();
	html->content().img().src(f1).width(w).height(h).end();
	html->content().end();
	// before/after
	html->content().end();
	// script
	html->content().script().type("text/javascript");
	html->content().text("$(function(){$('#" + id + "').beforeAfter();});");
	html->content().end();
	// end
	html->content().p().classAttr("test-notes").text("Notes: " + comment).end();
	html->content().br().end();
	html->content().center().a().href("#index").text("&#8593; back to top").end(2);
	html->content().end();
}
//-----------------------------------------------------------------------------
void setupEnv() {
	using namespace boost::filesystem;
	if ( !exists(OUTPUT_FOLDER) ) {
		create_directory(OUTPUT_FOLDER);
	}
	if ( !exists(OUTPUT_FOLDER+REFERENCE_FOLDER) ) {
		create_directory(OUTPUT_FOLDER+REFERENCE_FOLDER);
	}
}

} // namespace tests
