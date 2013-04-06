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

namespace sambag { namespace disco {
//=============================================================================
///////////////////////////////////////////////////////////////////////////////
// Helper
//-----------------------------------------------------------------------------
extern Rectangle getPatternRect(IPattern::Ptr p);
//-----------------------------------------------------------------------------
extern void alignPattern(IDrawContext::Ptr cn, IPattern::Ptr p, const Rectangle &dst);
}} // namespace(s)

#endif /* SAMBAG_DISCOHELPER_H */
