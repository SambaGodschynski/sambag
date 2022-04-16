/*
 * DiscoHelper.hpp
 *
 *  Created on: Sat Apr  6 15:16:56 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DISCOHELPER_H
#define SAMBAG_DISCOHELPER_H

#include <boost/shared_ptr.hpp>
#include "IPattern.hpp"
#include <sambag/disco/Geometry.hpp>
#include "IDrawContext.hpp"
#include <string>

namespace sambag { namespace disco {
//=============================================================================
///////////////////////////////////////////////////////////////////////////////
// Helper
//-----------------------------------------------------------------------------
extern Rectangle getPatternRect(IPattern::Ptr p);
//-----------------------------------------------------------------------------
/**
 * transforms pattern aligned to given rect.
 */
extern void alignPattern(IDrawContext::Ptr cn, IPattern::Ptr p, const Rectangle &dst);
//-----------------------------------------------------------------------------
/**
 * parses string pattern.
 * @return NULL if pattern doesn't match
 * 
 * pos: 0..1
 * colorstop : rrggbbaa:pos
 * lin(p1x*, p1y*, p2x, p2y,[colorstops])
 *   lin(0,0,0,1,[ff000000:0, ff1000fff0:1])
 *   lin(0,1,[colors]) => lin(0,0,0,1,[colorstops])
 * lin(c1x*, c1y*, c1r*, c2x, c2y, c2r, [colorstops])
 *   grad(0,0,0,0,0,100,[colorstops]) 
 *   grad(0,0,100,[colors]) => grad(0,0,0,0,0,100,[colorstops])
 */
extern IPattern::Ptr createPattern(const std::string &str);
//-----------------------------------------------------------------------------
extern bool isGradient(IPattern::Ptr);
}} // namespace(s)

#endif /* SAMBAG_DISCOHELPER_H */
