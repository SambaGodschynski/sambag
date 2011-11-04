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

namespace tests {

class TestSurface : public sambag::disco::ISurface {
public:
	typedef boost::shared_ptr<TestSurface> Ptr;
private:
	std::string name;
	TestSurface(const std::string &name) : name(name) {}
public:
	static Ptr create(const std::string &name) {
		Ptr neu(new TestSurface(name));
		return neu;
	}
	virtual sambag::disco::Rectangle getSize() const {
		return sambag::disco::Rectangle();
	}
	const std::string & getName() const {
		return name;
	}

};

extern const std::string OUTPUT_FOLDER;
extern const std::string REFERENCE_FOLDER;
extern const std::string COMPARE_SCRIPT;

extern bool comparePng( const std::string &f1, const std::string &f2);
extern void writePng(cairo_surface_t *surface, const std::string name);
extern void testPng( const std::string &testName, cairo_surface_t *surface );
extern void setupEnv();
} // namespace

#endif /* TESTHELPER_HPP_ */
