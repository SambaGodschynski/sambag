/*
 * MouseEvent.cpp
 *
 *  Created on: Mon May 21 15:06:00 2012
 *      Author: Johannes Unger
 */

#include "MouseEvent.hpp"
#include <sambag/disco/components/AComponent.hpp>

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
//  Class MouseEvent
//=============================================================================
//-----------------------------------------------------------------------------
MouseEvent::MouseEvent(const Point2D &p, int buttons, MouseEvent::Type type) :
	p(p),
	buttons(buttons),
	wheelRotation(0.f),
	type(type)
{
}
//-----------------------------------------------------------------------------
MouseEvent::MouseEvent(AComponentPtr source, const Point2D &p
	, int buttons, MouseEvent::Type type) :
	source(source),
	p(p),
	buttons(buttons),
	wheelRotation(0),
	type(type)
{
	if (source)
		screenLocation = source->getLocationOnScreen(p);
}
//-----------------------------------------------------------------------------
const Point2D & MouseEvent::getLocation() const {
	return p;
}
//-----------------------------------------------------------------------------
const Point2D & MouseEvent::getLocationOnScreen() const {
	return screenLocation;
}
//-----------------------------------------------------------------------------
int MouseEvent::getButtons() const {
	return buttons;
}
//-----------------------------------------------------------------------------
float MouseEvent::getWheelRotation() const {
	return wheelRotation;
}
//-----------------------------------------------------------------------------
AComponentPtr MouseEvent::getSource() const {
	return source;
}
//-----------------------------------------------------------------------------
MouseEvent::Type MouseEvent::getType() const {
	return type;
}
//-----------------------------------------------------------------------------
std::string MouseEvent::toString() const {
	return "MouseEvent(" + paramString() + ")";
}
//-----------------------------------------------------------------------------
std::string MouseEvent::paramString() const {
	std::stringstream ss;
	ss << (getSource() ? getSource()->getName() : std::string("no source"));
	ss <<", "<<p<<", ";
	if (type  == UNKNOWN_EVENT)
		ss<<"UNKNOWN_EVENT";
	if ((type & DISCO_MOUSE_CLICKED) == DISCO_MOUSE_CLICKED)
		ss<<"DISCO_MOUSE_CLICKED";
	if ((type & DISCO_MOUSE_DRAGGED) == DISCO_MOUSE_DRAGGED)
		ss<<"DISCO_MOUSE_DRAGGED";
	if ((type & DISCO_MOUSE_ENTERED) == DISCO_MOUSE_ENTERED)
		ss<<"DISCO_MOUSE_ENTERED";
	if ((type & DISCO_MOUSE_EXITED) == DISCO_MOUSE_EXITED)
		ss<<"DISCO_MOUSE_EXITED";
	if ((type & DISCO_MOUSE_MOVED) == DISCO_MOUSE_MOVED)
		ss<<"DISCO_MOUSE_MOVED";
	if ((type & DISCO_MOUSE_PRESSED) == DISCO_MOUSE_PRESSED)
		ss<<"DISCO_MOUSE_PRESSED";
	if ((type & DISCO_MOUSE_RELEASED) == DISCO_MOUSE_RELEASED)
		ss<<"DISCO_MOUSE_RELEASED";
	if ((type & DISCO_MOUSE_WHEEL) == DISCO_MOUSE_WHEEL)
		ss<<"DISCO_MOUSE_WHEEL";
	ss<<", "<<buttons<<", "<<getWheelRotation();
	return ss.str();
}
}}}} // namespace(s)
