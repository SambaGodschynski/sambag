/*
 * FontCache.cpp
 *
 *  Created on: Fri Jan 25 11:32:28 2013
 *      Author: Johannes Unger
 */

#include "FontCache.hpp"
#include <sambag/disco/genFormatter/GenFlowLayout.hpp>
#include <sambag/disco/genFormatter/GenericFormatter.hpp>
#include "IDiscoFactory.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>


namespace sambag { namespace disco {
typedef Loki::SingletonHolder<FontCache> FontCacheHolder;
//=============================================================================
//  Class FontCache
//=============================================================================
//-----------------------------------------------------------------------------
FontCache & FontCache::instance() {
	return FontCacheHolder::Instance();
}
namespace {
enum { CH_START = 32, CH_END= 127, CH_NUM = CH_END - CH_START + 1 };
inline size_t glyphToIndex(GlyphHelper::Glyph ch) {
	return ch-CH_START;
}
ISurface::Ptr _createFontMapImpl(IDrawContext::Ptr ftCn, 
	FontCache::GlyphMap &glm)
{
	FontCache::GlyphLocationMap	&glyphes = boost::get<1>(glm);
	FontCache::FontHeight &height = boost::get<2>(glm);
	height = 0;
	glyphes.resize(CH_NUM);
	typedef genFormatter::GenericFormatter< GlyphHelper,
		GlyphHelperAccess,
		genFormatter::GenFlowLayout
	> Formatter;
	Formatter f;
	f.setWidth(800.);
	//f.setHgap(ftCn->getCurrentFontHeight());
	//f.setVgap(ftCn->getCurrentFontHeight());
	for (size_t i=0; i<CH_NUM; ++i) {
		glyphes[i].glyph = CH_START + i;
		if ( i == 0 ) { // whitespace
			glyphes[i].glyph = 'o';
		}
		IDrawContext::TextExtends ex = ftCn->textExtendsX(glyphes[i].str());
		glyphes[i].w = ex.width;
		glyphes[i].h = ex.height;
		glyphes[i].offx = ex.x_bearing;
		glyphes[i].offy = ex.y_bearing;
		height = std::max(-ex.y_bearing, height);
		if ( glyphes[i].w==0 || glyphes[i].h==0 ) {
			glyphes[i].glyph = 0;
			continue;
		}
		f.addComponent(&glyphes[i]);
	}
	f.layout();

	if (f.getWidth() == 0 || f.getHeight() == 0) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			"creating fontmap failed."
		);
	}

	// draw result
	IDiscoFactory *fac = getDiscoFactory();
	ISurface::Ptr res = fac->createImageSurface(
		(Integer)ceil( f.getWidth() + f.getHgap() ),
		(Integer)ceil( f.getHeight() + f.getVgap()) 
	);
	IDrawContext::Ptr cn = fac->createContext(res);

	cn->setFont(ftCn->getCurrentFont());
	cn->setFillColor(ftCn->getFillColor());
	cn->setStrokeColor(ftCn->getStrokeColor());
	cn->setStrokeWidth(ftCn->getStrokeWidth());

	for (size_t i=0; i<CH_NUM; ++i) {
		const GlyphHelper &gl = glyphes[i];
		if (i == 0) {	// whitespace
			continue;
		}
		Coordinate x = gl.x;
		Coordinate y = gl.y;
		if (cn->getStrokeWidth()>0.) {
			cn->moveTo(Point2D(x-gl.offx, y-gl.offy));
			cn->textPath(gl.str());
			cn->stroke();
		}
		cn->moveTo(Point2D(x-gl.offx, y-gl.offy));
		cn->textPath(gl.str());
		cn->fill();
	}
	return res;
}
//typedef boost::tuple<ColorRGBA, ColorRGBA, Number, Font> FontTraits;
inline const ColorRGBA & getStrokeColor(const FontTraits &ft) {
	return boost::get<0>(ft);
}
inline const ColorRGBA & getFillColor(const FontTraits &ft) {
	return boost::get<1>(ft);
}
inline const Number & getStrokeWidth(const FontTraits &ft) {
	return boost::get<2>(ft);
}
inline const Font & getFont(const FontTraits &ft) {
	return boost::get<3>(ft);
}
} // namespace(s)
//-----------------------------------------------------------------------------
void FontCache::createGlyphMap(const FontTraits &ft, GlyphMap &map) {
	IDiscoFactory *fac = getDiscoFactory();
	IRecordingSurface::Ptr rec = fac->createRecordingSurface();
	IDrawContext::Ptr cn = fac->createContext(rec);
	cn->setFont(getFont(ft));
	cn->setFillColor(getFillColor(ft));
	cn->setStrokeColor(getStrokeColor(ft));
	cn->setStrokeWidth(getStrokeWidth(ft));
	
	boost::get<0>(map) = _createFontMapImpl(cn, map);	
}
//-----------------------------------------------------------------------------
const FontCache::GlyphMap & FontCache::getGlyphMap(const FontTraits &ft) {
	FontMap::iterator it = fontMap.find(ft);
	bool inserted;
	if (it!=fontMap.end()) {
		return it->second;
	}
	boost::tie(it, inserted) = 
		fontMap.insert(std::make_pair(ft, GlyphMap()));
	if (!inserted) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			"getGlyphMap failed."
		);
	}
	createGlyphMap(ft, it->second);
	return it->second;
}	
//-----------------------------------------------------------------------------
void FontCache::installFont(const FontTraits &ft) {
	getGlyphMap(ft);
}
//-----------------------------------------------------------------------------
void FontCache::drawText(IDrawContext::Ptr cn, const std::string &text) 
{
	cn->save();
	const GlyphMap &gl = getGlyphMap(createFontTraits(cn));
	ISurface::Ptr sf = boost::get<0>(gl);
	IDrawContext::Ptr sfCn = getDiscoFactory()->createContext(sf);
	const GlyphLocationMap &glm = boost::get<1>(gl);
	Point2D cursor = cn->getCurrentPoint();
	double fht = boost::get<2>(gl);
	for (size_t i=0; i<text.length(); ++i) {
		GlyphHelper::Glyph glyph = text[i];
		size_t index = glyphToIndex(glyph);
		if (index >= glm.size() ) {
			index = glyphToIndex('?');
		}
		const GlyphHelper &glh = glm[index];
		Coordinate x = glh.x;
		Coordinate y = glh.y;
		sfCn->copyAreaTo(cn, 
			Rectangle(x, y, glh.w, glh.h), 
			addTo(cursor, glh.offx, fht+glh.offy)
		);
		cursor.x( cursor.x() + glh.w ); 
	}
	cn->restore();
}
//-----------------------------------------------------------------------------
Rectangle FontCache::getTextBounds(IDrawContext::Ptr cn, const std::string &text) 
{
	const GlyphMap &gl = getGlyphMap(createFontTraits(cn));
	ISurface::Ptr sf = boost::get<0>(gl);
	IDrawContext::Ptr sfCn = getDiscoFactory()->createContext(sf);
	const GlyphLocationMap &glm = boost::get<1>(gl);
	Rectangle res(cn->getCurrentPoint(), 0., 0.);
	Point2D cursor = res.x0();
	double fht = boost::get<2>(gl);
	for (size_t i=0; i<text.length(); ++i) {
		GlyphHelper::Glyph glyph = text[i];
		size_t index = glyphToIndex(glyph);
		if (index >= glm.size() ) {
			index = glyphToIndex('?');
		}
		const GlyphHelper &glh = glm[index];
		cursor.x( cursor.x() + glh.w ); 
		cursor.y( std::max(Coordinate(res.x0().y() + glh.h), cursor.y()) ); 
	}
	res.x1(cursor);
	return res;
}
}} // namespace(s)

#include <sambag/disco/svg/Style.hpp> 

namespace sambag { namespace disco {
	size_t hash_value(const sambag::disco::FontTraits &o) {
		using namespace sambag::disco;
		size_t seed = 0;
		boost::hash_combine(seed, getStrokeColor(o));
		boost::hash_combine(seed, getFillColor(o));
		boost::hash_combine(seed, getStrokeWidth(o));
		boost::hash_combine(seed, getFont(o));
		return seed;
	}
}}