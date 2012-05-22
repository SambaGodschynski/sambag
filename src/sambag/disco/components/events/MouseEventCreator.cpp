/*
 * MouseEventCreator.cpp
 *
 *  Created on: Thu May 31 14:06:51 2012
 *      Author: Johannes Unger
 */

#include "MouseEventCreator.hpp"
#include <sambag/disco/components/RootPane.hpp>

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
//  Class MouseEventCreator
//=============================================================================
//-----------------------------------------------------------------------------
void
MouseEventCreator::createClickEvent(const Coordinate &x,
		const Coordinate &y, MouseEventCreator::Bitmask buttons)
{
	currEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::MOUSE_CLICKED);
	root->__processMouseEvent_(currEvent);
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createReleaseEvent(const Coordinate &x,
		const Coordinate &y, MouseEventCreator::Bitmask buttons)
{
	currEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::MOUSE_RELEASED);
	root->__processMouseEvent_(currEvent);
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createMoveEvent(const Coordinate &x,
		const Coordinate &y)
{


	if (currEvent.buttons>0 && !(currEvent.type == MouseEvent::MOUSE_RELEASED)) {
		currEvent =
			MouseEvent(root, Point2D(x, y), currEvent.buttons, MouseEvent::MOUSE_DRAGGED);
		root->__processMouseEvent_(currEvent);
		return;
	}
	currEvent = MouseEvent(root, Point2D(x, y), 0, MouseEvent::MOUSE_MOVED);
	root->__processMouseEvent_(currEvent);
}
}}}} // namespace(s)
