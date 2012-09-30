/*
 * GlyphCache.cpp
 *
 *  Created on: Tue Sep 25 06:04:00 2012
 *      Author: Johannes Unger
 */

#include "GlyphCache.hpp"
#include "IDiscoFactory.hpp"

namespace sambag { namespace disco { 
namespace {
	GlyphCache::Hash hash(IDrawContext::Ptr cn, GlyphCache::Char ch) {
		GlyphCache::Hash seed = 0;
		ColorRGBA col = cn->getFillColor();
		Font ft = cn->getCurrentFont();
		boost::hash_combine(seed, col.getR());
		boost::hash_combine(seed, col.getG());
		boost::hash_combine(seed, col.getB());
		boost::hash_combine(seed, col.getA());
		boost::hash_combine(seed, ft.fontFace);
		boost::hash_combine(seed, ft.weight);
		boost::hash_combine(seed, ft.slant);
		boost::hash_combine(seed, ft.size);
		boost::hash_combine(seed, ch);
		return seed;
	}
}
//=============================================================================
//  Class GlyphCache
//=============================================================================
//-----------------------------------------------------------------------------
IImageSurface::Ptr GlyphCache::createGlyphImage(IDrawContext::Ptr cn, Char ch) {
	IDiscoFactory * fac = getDiscoFactory();
	const Char c_str[] = {ch, '\0'};
	const std::string txt = std::string(c_str);
	Rectangle size = cn->textExtends(txt);
	IImageSurface::Ptr img = 
		fac->createImageSurface(size.getWidth(), size.getHeight());
	IDrawContext::Ptr imgCn = fac->createContext(img);
	imgCn->setFont(cn->getCurrentFont());
	imgCn->setFillColor(cn->getFillColor());
	imgCn->moveTo(Point2D(0,0));
	imgCn->textPath(txt);
	imgCn->fill();
	return img;
}
//-----------------------------------------------------------------------------
IImageSurface::Ptr GlyphCache::getGlyph(IDrawContext::Ptr cn, Char ch) {
	Hash k = hash(cn, ch);
	IImageSurface::Ptr img = glyphMap[k];
	if (!img) {
		img = createGlyphImage(cn, ch);
		glyphMap[k] = img;
	}
	return img;
}
//-----------------------------------------------------------------------------
void GlyphCache::drawText(IDrawContext::Ptr cn, const std::string &text) {
	for (size_t i=0; i<text.length(); ++i) {
		IImageSurface::Ptr img = getGlyph(cn, text[i]);
		cn->drawSurface(img);
		Rectangle sz = img->getSize();
		cn->translate(Point2D(sz.width(), 0));
		//loc.y( loc.y() + img->getSize().height() );
	}
}
///////////////////////////////////////////////////////////////////////////////
GlyphCache & getGlyphCache() {
	typedef Loki::SingletonHolder<GlyphCache> FactoryHolder;
	return FactoryHolder::Instance();
}
}} // namespace(s)

