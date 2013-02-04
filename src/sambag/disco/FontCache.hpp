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

namespace boost {
extern size_t hash_value(const sambag::disco::Font &o);
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
	typedef Font FontTraits;
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
}} // namespace(s)

#endif /* SAMBAG_FONTCACHE_H */
