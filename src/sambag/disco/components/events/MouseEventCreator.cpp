/*
 * MouseEventCreator.cpp
 *
 *  Created on: Thu May 31 14:06:51 2012
 *      Author: Johannes Unger
 */

#include "MouseEventCreator.hpp"
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
//  Class MouseEventCreator
//=============================================================================
//-----------------------------------------------------------------------------
void MouseEventCreator::fireEvent(const MouseEvent &ev) {
    if (!root) {
        SAMBAG_THROW(com::exceptions::IllegalStateException,"root==NULL");
    }
	EventSender<MouseEvent>::notifyListeners(this, ev);
	root->processMouseEvent(ev);
}
//-----------------------------------------------------------------------------
MouseEventCreator::Ptr MouseEventCreator::clone() const {
    Ptr res = MouseEventCreator::create();
    res->root = root;
    res->lastEvent = lastEvent;
    return res;
}
//-----------------------------------------------------------------------------
void MouseEventCreator::setRootPane(RootPanePtr root) {
    this->root = root;
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createPressEvent(const Coordinate &x,
		const Coordinate &y, MouseEventCreator::Bitmask buttons)
{
	lastEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::DISCO_MOUSE_PRESSED);
	fireEvent(lastEvent);
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createReleaseEvent(const Coordinate &x,
		const Coordinate &y, MouseEventCreator::Bitmask buttons)
{
	Point2D p = Point2D(x, y);
	if (lastEvent.getType() == MouseEvent::DISCO_MOUSE_PRESSED && lastEvent.p == p) {
		lastEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::DISCO_MOUSE_CLICKED);
		fireEvent( // 1. release
				MouseEvent(root, Point2D(x, y), buttons, MouseEvent::DISCO_MOUSE_RELEASED)
		);
		fireEvent(lastEvent); // 2. clicked
		return;
	}
	lastEvent = MouseEvent(root, Point2D(x, y), buttons, MouseEvent::DISCO_MOUSE_RELEASED);
	fireEvent(lastEvent);
}
//-----------------------------------------------------------------------------
void
MouseEventCreator::createMoveEvent(const Coordinate &x,
		const Coordinate &y)
{


	if (lastEvent.buttons>0
			&& !(lastEvent.type == MouseEvent::DISCO_MOUSE_RELEASED)
			&& !(lastEvent.type == MouseEvent::DISCO_MOUSE_CLICKED)) 
	{
		if ( lastEvent.p == Point2D(x,y)) {
			return;
		}
		lastEvent =
			MouseEvent(root, Point2D(x, y), lastEvent.buttons, MouseEvent::DISCO_MOUSE_DRAGGED);
		fireEvent(lastEvent);
		return;
	}
	lastEvent = MouseEvent(root, Point2D(x, y), 0, MouseEvent::DISCO_MOUSE_MOVED);
	fireEvent(lastEvent);
}
//-------------------------------------------------------------------------
void MouseEventCreator::
createWheelEvent(const Coordinate &x, const Coordinate &y, float wheelRotation)
{
	lastEvent = MouseEvent(root, Point2D(x, y), 0, MouseEvent::DISCO_MOUSE_WHEEL);
	lastEvent.updateWheelRotation(wheelRotation);
	fireEvent(lastEvent);
}
}}}} // namespace(s)
