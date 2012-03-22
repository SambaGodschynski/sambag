/*
 * TestHelper.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef TESTHELPER_HPP_
#define TESTHELPER_HPP_

#include <cairo.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include "sambag/disco/ISurface.hpp"
#include "sambag/disco/Geometry.hpp"
#include "sambag/xml/cpAtl/Html.hpp"
#include <iostream>
#include <fstream>
#include <time.h>

namespace tests {

/**
 * @struct TestSuiteHtmlOutput
 * creates TestSuite HTML page.
 */
class TestSuiteHtmlOutput {
public:
	typedef boost::shared_ptr<TestSuiteHtmlOutput> Ptr;
private:
	// one for doc
	sambag::xml::cpAtl::Html *_doc;
	// one for index
	sambag::xml::cpAtl::Html *_index;
	// and one for content
	sambag::xml::cpAtl::Html *_content;
	// also for stream
	std::stringstream sDoc;
	std::stringstream sIndex;
	std::stringstream sContent;
	std::string outfile;
	TestSuiteHtmlOutput(const std::string &outfile);
public:
	static Ptr create(const std::string &outfile);
	sambag::xml::cpAtl::Html & doc();
	sambag::xml::cpAtl::Html & index();
	sambag::xml::cpAtl::Html & content();
	~TestSuiteHtmlOutput();
};

extern const std::string OUTPUT_FOLDER;
extern const std::string REFERENCE_FOLDER;
extern const std::string COMPARE_SCRIPT;
//-----------------------------------------------------------------------------
extern bool comparePng( const std::string &f1, const std::string &f2);
//-----------------------------------------------------------------------------
extern void testPng( const std::string &testName, sambag::disco::IImageSurface::Ptr surface );
//-----------------------------------------------------------------------------
extern void testSvg( const std::string &testName,
		const std::string &inSvg,
		sambag::disco::IImageSurface::Ptr surface,
		TestSuiteHtmlOutput::Ptr html,
		const std::string &comment = std::string("-") );
//-----------------------------------------------------------------------------
extern void setupEnv();
//-----------------------------------------------------------------------------
inline std::string getTimeStampAsStr() {
	time_t t = time(NULL);
	char * ct = ctime(&t);
	ct[strlen(ct)-1] = '\0'; // remove \n
	return std::string(ct);
}
} // namespace

#endif /* TESTHELPER_HPP_ */
