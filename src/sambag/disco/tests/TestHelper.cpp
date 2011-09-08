/*
 * TestHelper.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "TestHelper.hpp"
#include <boost/filesystem.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <sambag/com/Common.hpp>
#include <cairo.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tests {

const std::string OUTPUT_FOLDER = "output/";
const std::string MR = "soll_";
const std::string COMPARE_SCRIPT = "sh " + OUTPUT_FOLDER + "comparepng.script ";

// make reference pictures for compare
//#define DISCO_CONTEXT_MAKE_REFERENCES
//-----------------------------------------------------------------------------
bool comparePng( const std::string &f1, const std::string &f2) {
	std::string cmd(COMPARE_SCRIPT + f1 + " " + f2 );
	int r = system(cmd.c_str());
	return r == 0;
}
//-----------------------------------------------------------------------------
void writePng(cairo_surface_t *surface, const std::string name) {
	cairo_surface_write_to_png(surface, name.c_str() );
}
//-----------------------------------------------------------------------------
void testPng( const std::string &testName, cairo_surface_t *surface ) {
	const std::string f1( OUTPUT_FOLDER + MR + testName + ".png" );
	const std::string f2( OUTPUT_FOLDER + testName + ".png" );

#ifdef DISCO_CONTEXT_MAKE_REFERENCES
	writePng(surface, f1);
#else
	writePng(surface, f2);
	CPPUNIT_ASSERT ( comparePng(f1, f2) );
#endif
}
//-----------------------------------------------------------------------------
void setupEnv() {
	using namespace boost::filesystem;
	if ( !exists(OUTPUT_FOLDER) ) {
		create_directory(OUTPUT_FOLDER);
	}
}

} // namespace tests
