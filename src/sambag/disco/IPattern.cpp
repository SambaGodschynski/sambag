/*
 * Pattern.cpp
 *
 *  Created on: Nov 30, 2011
 *      Author: samba
 */

#include "IPattern.hpp"

namespace sambag { namespace disco {
//-----------------------------------------------------------------------------
Rectangle ISolidPattern::getBounds() const {
    return NULL_RECTANGLE;
}
//-----------------------------------------------------------------------------
Rectangle ILinearPattern::getBounds() const {
    LinearPoints p = getLinearPoints();
    return Rectangle(boost::get<0>(p), boost::get<1>(p), false);
}
//-----------------------------------------------------------------------------
Rectangle IRadialPattern::getBounds() const {
    Point2D c0, c1;
    Number r0, r1;
    boost::tie(c0, r0, c1, r1) = getRadialCircles();
    return Rectangle(Point2D(0, 0), Point2D(r1*2,r1*2), false);
}
//-----------------------------------------------------------------------------
Rectangle ISurfacePattern::getBounds() const {
    return NULL_RECTANGLE;
}

}} // namespace(s)
