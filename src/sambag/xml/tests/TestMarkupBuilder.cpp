/*
 * TestMarkupBuilder.cpp
 *
 *  Created on: Dec 10, 2011
 *      Author: samba
 */

#include "TestMarkupBuilder.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <string>
#include <sstream>
#include <iostream>
#include "sambag/xml/cpAtl/SimpleTagCreator.hpp"
#include "sambag/xml/cpAtl/Html.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestMarkupBuilder );

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// tests
namespace tests {
//-----------------------------------------------------------------------------
void TestMarkupBuilder::testSimpleTagCreator() {
	using namespace sambag::xml::cpAtl;
	std::stringstream ss;
	SimpleTagCreator tc(ss);
	tc.addTag("svg").attr("viewBox", "0 0 300 300");
	tc.addTag("g");
	tc.addTag("rect").attr("width", "200").end();
	tc.addTag("text").text("hallo").end();
	tc.endAll();
	std::string exp = "<svg viewBox=\'0 0 300 300\'>\n"
			"	<g>\n"
			"		<rect width=\'200\'/>\n"
			"		<text>\n"
			"hallo\n"
			"		</text>\n"
			"	</g>\n"
			"</svg>";
	CPPUNIT_ASSERT_EQUAL(exp, ss.str());
}
//-----------------------------------------------------------------------------
void TestMarkupBuilder::testHtmlCreator() {
	using namespace sambag::xml::cpAtl;
	std::stringstream ss;
	Html html(ss);
	html.html().head().title().text("krll krll").end();
	html.body().h1().text("Hello").end();
	html.p().text("abcdefg");
	html.endAll();
	std::string exp = "<html>\n"
			"	<head>\n"
			"		<title>\n"
			"krll krll\n"
			"		</title>		<body>\n"
			"			<h1>\n"
			"Hello\n"
			"			</h1>			<p>\n"
			"abcdefg\n"
			"			</p>\n"
			"		</body>\n"
			"	</head>\n"
			"</html>";
	CPPUNIT_ASSERT_EQUAL(exp, ss.str());
}

} // namespace tests

