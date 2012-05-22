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
bool Graphics::hitClip(const Rectangle &r) const {
	// need to cast to Rectangle::Base (boost::geometry::box)
	// otherwise compiling fails.
	return boost::geometry::intersects<Rectangle::Base, Rectangle::Base>(r,
			clipExtends());
}
}}} // namespace(s)
