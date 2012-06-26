/*
 * Graphics.cpp
 *
 *  Created on: Wed May 23 11:06:47 2012
 *      Author: Johannes Unger
 */

#include "Graphics.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Graphics
//=============================================================================
//-----------------------------------------------------------------------------
void Graphics::clearRect(const Rectangle &r) {
	rect(r);
	fill();
}
//-----------------------------------------------------------------------------
void Graphics::clipRect(const Rectangle &r) {
	Rectangle res = clipExtends();
	boost::geometry::intersection<Rectangle::Base, Rectangle::Base,
		Rectangle::Base> (res, r, res);
	setClip(res);
}
//-----------------------------------------------------------------------------
bool Graphics::hitClip(const Rectangle &r) const {
	// need to cast to Rectangle::Base (boost::geometry::box)
	// otherwise compiling fails.
	return boost::geometry::intersects<Rectangle::Base, Rectangle::Base>(r,
			clipExtends());
}
//-----------------------------------------------------------------------------
void Graphics::copyArea(const Rectangle &src, const Point2D &d) {
	Point2D where = src.x0();
	boost::geometry::add_point(where, d);
	copyAreaTo(cn, src, where);
}
}}} // namespace(s)
