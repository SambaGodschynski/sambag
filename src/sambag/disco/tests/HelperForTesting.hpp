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

namespace tests {

extern const std::string OUTPUT_FOLDER;
extern const std::string REFERENCE_FOLDER;
extern const std::string COMPARE_SCRIPT;

// make reference pictures for compare
//#define DISCO_CONTEXT_MAKE_REFERENCES
extern bool comparePng( const std::string &f1, const std::string &f2);
extern void writePng(cairo_surface_t *surface, const std::string name);
extern void testPng( const std::string &testName, cairo_surface_t *surface );
extern void setupEnv();
} // namespace

#endif /* TESTHELPER_HPP_ */
