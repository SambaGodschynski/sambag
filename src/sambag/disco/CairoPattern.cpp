/*
 * CairoPattern.cpp
 *
 *  Created on: Dec 15, 2011
 *      Author: samba
 */

#include "CairoPattern.hpp"
#include "CairoSurface.hpp"

namespace sambag { namespace disco {
//=============================================================================
namespace gradient {
//=============================================================================
//-----------------------------------------------------------------------------
void getStops( CairoPatternRef ref,
	IGradient::ColorStops &out,
		size_t startIndex,
		size_t _endIndex)
{
	int endIndex = _endIndex;
	if (endIndex==INT_MAX) {
		cairo_pattern_get_color_stop_count(ref.get(),
				&endIndex);
	}
	if (startIndex>_endIndex) // important for delegate.reserve()
		return;
	out.reserve(endIndex-startIndex);
	for(int i=startIndex; i<endIndex; ++i) {
		out.push_back(getColorStop(ref, i));
	}
}
//-----------------------------------------------------------------------------
void addColorStops(CairoPatternRef ref, 
	const IGradient::ColorStops &newStops)
{
	boost_for_each(const IGradient::ColorStop &col, newStops) {
		addColorStop(ref, col);
	}
}
//-----------------------------------------------------------------------------
void addColorStop ( CairoPatternRef ref,
	const IGradient::ColorStop &colStop ) 
{
	ColorRGBA col;
	Number offset;
	boost::tie(col, offset) = colStop;
	cairo_pattern_add_color_stop_rgba( ref.get(),
		offset,
		col.getR(),
		col.getG(),
		col.getB(),
		col.getA()
	);
}
//-----------------------------------------------------------------------------
void addColorStop ( CairoPatternRef ref,
	const ColorRGBA &col, 
	const Number &offset ) 
{
	addColorStop(ref, IGradient::ColorStop(col, offset));
}
//-----------------------------------------------------------------------------
size_t getNumStop(CairoPatternRef ref) {
	int res;
	cairo_pattern_get_color_stop_count(ref.get(), &res);
	return res;
}
//-----------------------------------------------------------------------------
IGradient::ColorStop getColorStop ( CairoPatternRef ref, 
	size_t index ) 
{
	Number offset, r, g, b, a;
	cairo_pattern_get_color_stop_rgba(ref.get(), index,
		&offset, &r, &g, &b, &a);
	return IGradient::ColorStop(ColorRGBA(r,g,b,a), offset);
}
} // namespace gradient
//=============================================================================
// class CairoSurfacePattern
//=============================================================================
//-----------------------------------------------------------------------------
CairoSurfacePattern::Ptr CairoSurfacePattern::create(ISurface::Ptr _surf) {
	CairoSurface::Ptr surf =
		boost::shared_dynamic_cast<CairoSurface>(_surf);
	if (!surf)
		return Ptr();
	CairoPatternRef ref = createPatternRef(
		cairo_pattern_create_for_surface(surf->getCairoSurface())
	);
	Ptr neu(new CairoSurfacePattern(ref));
	neu->surface = surf;
	return neu;
}
}}
