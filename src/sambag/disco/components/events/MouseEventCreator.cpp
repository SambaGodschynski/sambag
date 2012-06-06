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
MouseEventCreator::createPressEvent(const Coordinate &x,
		const Coordinate &y, MouseEventCreator::Bitmask buttons)
{
	lastEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::MOUSE_PRESSED);
	root->__processMouseEvent_(lastEvent);
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createReleaseEvent(const Coordinate &x,
		const Coordinate &y, MouseEventCreator::Bitmask buttons)
{
	Point2D p = Point2D(x, y);
	if (lastEvent.getType() == MouseEvent::MOUSE_PRESSED && lastEvent.p == p) {
		lastEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::MOUSE_CLICKED);
		root->__processMouseEvent_( // 1. release
				MouseEvent(root, Point2D(x, y), buttons, MouseEvent::MOUSE_RELEASED)
		);
		root->__processMouseEvent_(lastEvent); // 2. clicked
		return;
	}
	lastEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::MOUSE_RELEASED);
	root->__processMouseEvent_(lastEvent);
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createMoveEvent(const Coordinate &x,
		const Coordinate &y)
{


	if (lastEvent.buttons>0
			&& !(lastEvent.type == MouseEvent::MOUSE_RELEASED)
			&& !(lastEvent.type == MouseEvent::MOUSE_CLICKED)) {
		lastEvent =
			MouseEvent(root, Point2D(x, y), lastEvent.buttons, MouseEvent::MOUSE_DRAGGED);
		root->__processMouseEvent_(lastEvent);
		return;
	}
	lastEvent = MouseEvent(root, Point2D(x, y), 0, MouseEvent::MOUSE_MOVED);
	root->__processMouseEvent_(lastEvent);
}
}}}} // namespace(s)
