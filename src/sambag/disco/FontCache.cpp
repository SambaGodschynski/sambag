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
void _createFontMapImpl(IDrawContext::Ptr cn, FontCache::GlyphLocationMap &glyphes)
{
	glyphes.resize(CH_NUM);
	typedef genFormatter::GenericFormatter< GlyphHelper,
		GlyphHelperAccess,
		genFormatter::GenFlowLayout
	> Formatter;
	Formatter f;
	f.setWidth(800.);
	f.setHgap(cn->getCurrentFontHeight());
	f.setVgap(cn->getCurrentFontHeight());
	for (size_t i=0; i<CH_NUM; ++i) {
		glyphes[i].glyph = CH_START + i;
		IDrawContext::TextExtends ex = cn->textExtendsX(glyphes[i].str());
		glyphes[i].w = ex.width;
		glyphes[i].h = ex.height;
		if ( glyphes[i].w==0 || glyphes[i].h==0 ) {
			glyphes[i].glyph = 0;
			continue;
		}
		f.addComponent(&glyphes[i]);
	}
	f.layout();
	// draw result
	for (size_t i=0; i<CH_NUM; ++i) {
		const GlyphHelper &gl = glyphes[i];
		if (gl.glyph == 0) {
			continue;
		}
		cn->moveTo(Point2D(gl.x, gl.y + gl.h));
		cn->textPath(gl.str());
		cn->fill();
		
	}
}
} // namespace(s)
//-----------------------------------------------------------------------------
void FontCache::createGlyphMap(const Font &ft, GlyphMap &map) {
	IDiscoFactory *fac = getDiscoFactory();
	IRecordingSurface::Ptr rec = fac->createRecordingSurface();
	IDrawContext::Ptr cn = fac->createContext(rec);

	cn->setFont(ft);
	cn->setFillColor(ColorRGBA(1,1,1,1));
	cn->setFillColor(ColorRGBA(0));
	_createFontMapImpl(cn, map.second);

	Rectangle size = rec->getSize();

	if (size.width() == 0 || size.height() == 0) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			"creating fontmap failed."
		);
	}

	map.first = fac->createImageSurface((Integer)size.width(),
		(Integer)size.height());
	fac->createContext(map.first)->drawSurface(rec);
}
//-----------------------------------------------------------------------------
const FontCache::GlyphMap & FontCache::getGlyphMap(const Font &ft) {
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
void FontCache::installFont(const Font &ft) {
	getGlyphMap(ft);
}
//-----------------------------------------------------------------------------
void FontCache::drawText(IDrawContext::Ptr cn, const std::string &text) {
	const GlyphMap &gl = getGlyphMap(cn->getCurrentFont());
	ISurface::Ptr sf = gl.first;
	IDrawContext::Ptr sfCn = getDiscoFactory()->createContext(sf);
	const GlyphLocationMap &glm = gl.second;
	Point2D cursor = cn->getCurrentPoint();
	for (size_t i=0; i<text.length(); ++i) {
		GlyphHelper::Glyph glyph = text[i];
		size_t index = glyphToIndex(glyph);
		if (index >= glm.size() ) {
			index = glyphToIndex('?');
		}
		const GlyphHelper &glh = glm[index];
		sfCn->copyAreaTo(cn, Rectangle(glh.x, glh.y, glh.w, glh.h), cursor);
		cursor.x( cursor.x() + glh.w ); 
	}
}
}} // namespace(s)
