/*
 * MouseEventRecorder.cpp
 *
 *  Created on: Wed Jul  3 21:35:17 2013
 *      Author: Johannes Unger
 */

#include "MouseEventRecorder.hpp"

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
//  Class MouseEventRecorder
//=============================================================================
//-----------------------------------------------------------------------------
MouseEventRecorder::Super::Ptr
MouseEventRecorder::clone() const
{
    Ptr res = create();
    copyFields(res);
    return res;
}
//-----------------------------------------------------------------------------
MouseEventRecorder::Ptr MouseEventRecorder::create()
{
    Ptr res = Ptr( new MouseEventRecorder() );
    return res;
}
//-----------------------------------------------------------------------------
void
MouseEventRecorder::createPressEvent(const Coordinate &x,
                     const Coordinate &y, Bitmask buttons)
{
    std::cout<<x<<","<<y<<std::endl;
    Super::createPressEvent(x,y,buttons);
}
//-----------------------------------------------------------------------------
void
MouseEventRecorder::createReleaseEvent(const Coordinate &x,
                    const Coordinate &y, Bitmask buttons)
{
    Super::createReleaseEvent(x,y,buttons);
}
//-----------------------------------------------------------------------------
void
MouseEventRecorder::createMoveEvent(const Coordinate &x, const Coordinate &y)
{
    Super::createMoveEvent(x, y);
}
//-----------------------------------------------------------------------------
void
MouseEventRecorder::createWheelEvent(const Coordinate &x,
                                     const Coordinate &y, float wheelRotation)
{
    Super::createWheelEvent(x,y,wheelRotation);
}

}}}} // namespace(s)
