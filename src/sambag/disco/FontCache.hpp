/*
 * FontCache.hpp
 *
 *  Created on: Fri Jan 25 11:32:28 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_FONTCACHE_H
#define SAMBAG_FONTCACHE_H

#include <loki/Singleton.h>
#include "ISurface.hpp"
#include "IDrawContext.hpp"
#include <string>
#include "GlyphHelper.hpp"
#include <boost/tuple/tuple.hpp>


namespace sambag { namespace disco {
	typedef boost::tuple<sambag::disco::ColorRGBA, 
		sambag::disco::ColorRGBA, 
		sambag::com::Number, 
		sambag::disco::Font
	> FontTraits; 
}}

namespace boost {
	extern size_t hash_value(const sambag::disco::FontTraits &o);
}

#include <boost/unordered_map.hpp>

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class FontCache.
  * To understand as workaround for slow text drawing in cairo.
  */
class FontCache {
//=============================================================================
friend struct Loki::CreateUsingNew<FontCache>;
public:
	//-------------------------------------------------------------------------
	typedef std::vector<GlyphHelper> GlyphLocationMap;
	//-------------------------------------------------------------------------
	typedef double FontHeight;
	//-------------------------------------------------------------------------
	typedef boost::tuple<ISurface::Ptr, GlyphLocationMap, FontHeight> GlyphMap;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<FontTraits, GlyphMap> FontMap;
protected:
	//-------------------------------------------------------------------------
	void createGlyphMap(const FontTraits &ft, GlyphMap &map);
private:
	//-------------------------------------------------------------------------
	FontMap fontMap;
public:
	//-------------------------------------------------------------------------
	void installFont(const FontTraits &ft);
	//-------------------------------------------------------------------------
	const GlyphMap & getGlyphMap(const FontTraits &ft);
	//-------------------------------------------------------------------------
	static FontCache & instance();
	//-------------------------------------------------------------------------
	Rectangle getTextBounds(IDrawContext::Ptr cn, const std::string &text);
	//-------------------------------------------------------------------------
	void drawText(IDrawContext::Ptr cn, const std::string &text);
}; // FontCache
//-----------------------------------------------------------------------------
inline FontTraits createFontTraits(IDrawContext::Ptr cn) {
	return FontTraits(cn->getStrokeColor(),
		cn->getFillColor(),
		cn->getStrokeWidth(),
		cn->getCurrentFont()
	);
}
//-----------------------------------------------------------------------------
inline FontTraits createFontTraits(const ColorRGBA &strokeColor, 
	const ColorRGBA &fillColor, Number strokeWidth, const Font &fnt) 
{
	return FontTraits(strokeColor,
		fillColor,
		strokeWidth,
		fnt
	);
}
//-----------------------------------------------------------------------------
inline FontTraits createFontTraits(const ColorRGBA &color, const Font &fnt) 
{
	return FontTraits( ColorRGBA(),
		color,
		0,
		fnt
	);
}
}} // namespace(s)

#endif /* SAMBAG_FONTCACHE_H */
