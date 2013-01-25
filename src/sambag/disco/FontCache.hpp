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
#include <boost/unordered_map.hpp>

namespace boost {
extern size_t hash_value(const sambag::disco::Font &o);
}

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
	typedef std::pair<ISurface::Ptr, GlyphLocationMap> GlyphMap;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Font, GlyphMap> FontMap;
protected:
	//-------------------------------------------------------------------------
	void createGlyphMap(const Font &ft, GlyphMap &map);
private:
	//-------------------------------------------------------------------------
	FontMap fontMap;
public:
	//-------------------------------------------------------------------------
	void installFont(const Font &ft);
	//-------------------------------------------------------------------------
	const GlyphMap & getGlyphMap(const Font &ft);
	//-------------------------------------------------------------------------
	static FontCache & instance();
	//-------------------------------------------------------------------------
	void drawText(IDrawContext::Ptr cn, const std::string &text);
}; // FontCache
}} // namespace(s)

#endif /* SAMBAG_FONTCACHE_H */
