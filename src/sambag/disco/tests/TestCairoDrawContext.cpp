/*
 * TestCairoDrawContext.cpp
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#include "TestCairoDrawContext.hpp"
#include <cppunit/config/SourcePrefix.h>
#include "cairo/cairo-svg.h"
#include <boost/filesystem.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <sambag/com/Common.hpp>

using namespace sambag::com;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestCairoDrawContext );

cairo_status_t
write_func (void *pStringstream, unsigned char *data, unsigned int length)
{
	std::stringstream &ss = *(reinterpret_cast<std::stringstream*>(pStringstream));
	std::string strData((const char*)data, length);
	ss<<strData;
	return CAIRO_STATUS_SUCCESS;
}

void writeSvg ( const std::stringstream &stream, const std::string filename ) {
	using namespace std;
	std::fstream fstr;
	fstr.open (filename.c_str(), fstream::out );
	fstr<<stream.str();
	fstr.close();
}

cairo_surface_t * createSvgSurface( std::stringstream &ss, const Number &width, const Number &height ) {
	cairo_write_func_t f = (cairo_write_func_t)write_func;
	cairo_surface_t *surface =
	cairo_svg_surface_create_for_stream( f, &ss, width, height );
	return surface;
}

// TODO: create shared_ptr wrapper for surface
void releaseSurface( cairo_surface_t *surface ) {
	cairo_surface_destroy(surface);
}

int compareStr( const std::string &s1, const std::string &s2 ) {
	if ( s1.length() != s2.length() ) return -1;
	for ( size_t i=0; i<s1.length(); ++i ) {
		if ( s1.at(i) != s2.at(i) ) return i;
	}
	return 0;
}


namespace tests {
//=============================================================================
// TestCairoDrawContext::testConstructor
//=============================================================================
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testConstructor() {
	using namespace sambag::disco;
	using namespace boost;
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 320, 200);
	CairoDrawContext::Ptr context = CairoDrawContext::create( surface );
	cairo_surface_write_to_png(surface, "testConstructor.png");
	context.reset();
	cairo_surface_destroy(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>check file exists
	CPPUNIT_ASSERT( filesystem::exists("testConstructor.png") );
	filesystem::remove("testConstructor.png");

}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testLine() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> expected svg
	static const std::string EXP = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"1200pt\" height=\"400pt\" viewBox=\"0 0 1200 400\" version=\"1.1\">\n"
			"<g id=\"surface2\">\n"
			"<path style=\"fill:none;stroke-width:5;stroke-linecap:butt;stroke-linejoin:miter;stroke:rgb(0%,0%,0%);stroke-opacity:1;stroke-miterlimit:10;\" d=\"M 100 300 L 300 100 \"/>\n"
			"<path style=\"fill:none;stroke-width:10;stroke-linecap:butt;stroke-linejoin:miter;stroke:rgb(0%,0%,0%);stroke-opacity:1;stroke-miterlimit:10;\" d=\"M 300 300 L 500 100 \"/>\n"
			"<path style=\"fill:none;stroke-width:15;stroke-linecap:butt;stroke-linejoin:miter;stroke:rgb(0%,0%,0%);stroke-opacity:1;stroke-miterlimit:10;\" d=\"M 500 300 L 700 100 \"/>\n"
			"<path style=\"fill:none;stroke-width:20;stroke-linecap:butt;stroke-linejoin:miter;stroke:rgb(0%,0%,0%);stroke-opacity:1;stroke-miterlimit:10;\" d=\"M 700 300 L 900 100 \"/>\n"
			"<path style=\"fill:none;stroke-width:25;stroke-linecap:butt;stroke-linejoin:miter;stroke:rgb(0%,0%,0%);stroke-opacity:1;stroke-miterlimit:10;\" d=\"M 900 300 L 1100 100 \"/>\n"
			"</g>\n"
			"</svg>\n";
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> svg as stream
	std::stringstream ss;
	cairo_surface_t *surface = createSvgSurface( ss, 1200.0, 400.0 );
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setStrokeWidth(5.0);
	context->drawLine( Point2D(100, 300), Point2D(300, 100)  );
	context->setStrokeWidth(10.0);
	context->drawLine( Point2D(300, 300), Point2D(500, 100)  );
	context->setStrokeWidth(15.0);
	context->drawLine( Point2D(500, 300), Point2D(700, 100)  );
	context->setStrokeWidth(20.0);
	context->drawLine( Point2D(700, 300), Point2D(900, 100)  );
	context->setStrokeWidth(25.0);
	context->drawLine( Point2D(900, 300), Point2D(1100, 100)  );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>release context
	context.reset();
	releaseSurface(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> compareOutput
	CPPUNIT_ASSERT_EQUAL( EXP, ss.str() );
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testArc() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> expected svg
	static const std::string EXP = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"1200pt\" height=\"400pt\" viewBox=\"0 0 1200 400\" version=\"1.1\">\n"
			"<g id=\"surface6\">\n"
			"<path style=\"fill:none;stroke-width:10;stroke-linecap:butt;stroke-linejoin:miter;stroke:rgb(100%,0%,0%);stroke-opacity:1;stroke-miterlimit:10;\" d=\"M 700 200 C 700 255.226562 655.226562 300 600 300 C 544.773438 300 500 255.226562 500 200 C 500 144.773438 544.773438 100 600 100 C 655.226562 100 700 144.773438 700 200 \"/>\n"
			"<path style=\" stroke:none;fill-rule:nonzero;fill:rgb(0%,0%,100%);fill-opacity:1;\" d=\"M 700 200 C 700 255.226562 655.226562 300 600 300 C 544.773438 300 500 255.226562 500 200 C 500 144.773438 544.773438 100 600 100 C 655.226562 100 700 144.773438 700 200 \"/>\n"
			"</g>\n"
			"</svg>\n";
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> svg as stream
	std::stringstream ss;
	cairo_surface_t *surface = createSvgSurface( ss, 1200.0, 400.0 );
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> draw
	context->setStrokeWidth(10);
	context->setStrokeColor( ColorRGBA( 1.0, 0, 0 ) );
	context->drawArc( Point2D(600, 200), 100 );
	context->setFillColor( ColorRGBA( 0, 0, 1.0 ) );
	context->fillArc( Point2D(600, 200), 100 );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>release context
	context.reset();
	releaseSurface(surface);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> compareOutput
	writeSvg(ss, "tmpOut.svg");
	CPPUNIT_ASSERT_EQUAL( EXP, ss.str() );
}
//-----------------------------------------------------------------------------
void TestCairoDrawContext::testSetWidth() {
	using namespace sambag::disco;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> svg as stream
	std::stringstream ss;
	cairo_surface_t *surface = createSvgSurface( ss, 1200.0, 400.0 );
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
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> svg as stream
	std::stringstream ss;
	cairo_surface_t *surface = createSvgSurface( ss, 1200.0, 400.0 );
	IDrawContext::Ptr context = CairoDrawContext::create( surface );
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set/compare
	context->setStrokeColor( ColorRGBA( 0.5, 0.25, 0.125 ) );
	CPPUNIT_ASSERT ( ColorRGBA( 0.5, 0.25, 0.125 ) == context->getStrokeColor() );

	context->setFillColor( ColorRGBA( 0.5, 0.25, 0.125 ) );
	CPPUNIT_ASSERT ( ColorRGBA( 0.5, 0.25, 0.125 ) == context->getFillColor() );
	//
	context.reset();
	releaseSurface(surface);
}


} //namespaces
