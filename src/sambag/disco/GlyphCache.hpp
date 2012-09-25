/*
 * GlyphCache.hpp
 *
 *  Created on: Tue Sep 25 06:04:00 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GLYPHCACHE_H
#define SAMBAG_GLYPHCACHE_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <loki/Singleton.h>
#include "ISurface.hpp"
#include "IDrawContext.hpp"
#include <string>

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class GlyphCache.
  */
class GlyphCache {
//=============================================================================
friend struct Loki::CreateUsingNew<GlyphCache>;
public:
	//-------------------------------------------------------------------------
	typedef size_t Hash;
	//-------------------------------------------------------------------------
	typedef char Char;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Hash, IImageSurface::Ptr> GlyphMap;
protected:
	//-------------------------------------------------------------------------
	GlyphCache(){}
	//-------------------------------------------------------------------------
	GlyphMap glyphMap;
	//-------------------------------------------------------------------------
	IImageSurface::Ptr createGlyphImage(IDrawContext::Ptr cn, Char ch);
	//-------------------------------------------------------------------------
	IImageSurface::Ptr getGlyph(IDrawContext::Ptr cn, Char ch);
public:
	//-------------------------------------------------------------------------
	void drawText(IDrawContext::Ptr cn, const std::string &text);

}; // GlyphCache
///////////////////////////////////////////////////////////////////////////////
extern GlyphCache & getGlyphCache();

}} // namespace(s)

#endif /* SAMBAG_GLYPHCACHE_H */
