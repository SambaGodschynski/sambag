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
	type(type)
{
}
//-----------------------------------------------------------------------------
MouseEvent::MouseEvent(AComponentPtr source, const Point2D &p
	, int buttons, MouseEvent::Type type) :
	source(source),
	p(p),
	buttons(buttons),
	type(type)
{
}
//-----------------------------------------------------------------------------
MouseEvent::MouseEvent(AComponentPtr source, const MouseEvent &ev) :
	source(source),
	p(ev.p),
	buttons(ev.buttons),
	type(ev.type)
{
}
//-----------------------------------------------------------------------------
const Point2D & MouseEvent::getLocation() const {
	return p;
}
//-----------------------------------------------------------------------------
int MouseEvent::getButtons() const {
	return buttons;
}
//-----------------------------------------------------------------------------
AComponentPtr MouseEvent::getSource() const {
	return source;
}
//-----------------------------------------------------------------------------
MouseEvent::Type MouseEvent::getType() const {
	return type;
}
//-------------------------------------------------------------------------
std::string MouseEvent::toString() const {
	return "MouseEvent(" + paramString() + ")";
}
//-------------------------------------------------------------------------
std::string MouseEvent::paramString() const {
	std::stringstream ss;
	ss<<source->getName()<<", "<<p<<", ";
	switch(type) {
	case MOUSE_CLICKED:
		ss<<"MOUSE_CLICKED"; break;
	case MOUSE_DRAGGED:
	ss<<"MOUSE_DRAGGED"; break;
	case MOUSE_ENTERED:
		ss<<"MOUSE_ENTERED"; break;
	case MOUSE_EXITED:
		ss<<"MOUSE_EXITED"; break;
	case MOUSE_MOVED:
		ss<<"MOUSE_MOVED"; break;
	case MOUSE_PRESSED:
		ss<<"MOUSE_PRESSED"; break;
	case MOUSE_RELEASED:
		ss<<"MOUSE_RELEASED"; break;
	case MOUSE_WHEEL:
		ss<<"MOUSE_WHEEL"; break;
	case NONE:
		ss<<"NONE"; break;
	}
	ss<<", "<<buttons;
	return ss.str();
}
}}}} // namespace(s)
