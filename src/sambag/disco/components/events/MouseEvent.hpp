/*
 * MouseEvent.hpp
 *
 *  Created on: Mon May 21 15:06:00 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MOUSEEVENT_H
#define SAMBAG_MOUSEEVENT_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/disco/Geometry.hpp>
#include "DiscoEvent.hpp"
namespace sambag { namespace disco { namespace components { namespace events {

//=============================================================================
/** 
  * @class MouseEvent.
  */
class MouseEvent : public DiscoEvent {
friend class MouseEventCreator;
//=============================================================================
public:
	//-------------------------------------------------------------------------
	// Indicates mouse button #1; used by getButton().
	static const int	DISCO_BTN1 = 1;
	// Indicates mouse button #2; used by getButton().
	static const int	DISCO_BTN2 = 1<<1;
	// Indicates mouse button #3; used by getButton().
	static const int	DISCO_BTN3 = 1<<2;
	enum Type {
		UNKNOWN_EVENT,
		DISCO_MOUSE_CLICKED = 1,
		DISCO_MOUSE_DRAGGED = 1 << 1,
		DISCO_MOUSE_ENTERED = 1 << 2,
		DISCO_MOUSE_EXITED = 1 << 3,
		DISCO_MOUSE_MOVED = 1 << 4,
		DISCO_MOUSE_PRESSED = 1 << 5,
		DISCO_MOUSE_RELEASED = 1 << 6,
		DISCO_MOUSE_WHEEL = 1 << 7,
		ALL_EVENTS = DISCO_MOUSE_PRESSED | DISCO_MOUSE_RELEASED | 
			DISCO_MOUSE_CLICKED | DISCO_MOUSE_MOVED | 
			DISCO_MOUSE_DRAGGED | DISCO_MOUSE_ENTERED | 
			DISCO_MOUSE_EXITED | DISCO_MOUSE_WHEEL
	};
private:
	//-------------------------------------------------------------------------
	AComponentPtr source;
	//-------------------------------------------------------------------------
	Point2D p;
	//-------------------------------------------------------------------------
	Point2D screenLocation;
	//-------------------------------------------------------------------------
	int buttons;
	//-------------------------------------------------------------------------
	float wheelRotation;
	//-------------------------------------------------------------------------
	Type type;
public:
	//-------------------------------------------------------------------------
	MouseEvent() : buttons(0), type(UNKNOWN_EVENT) {};
	//-------------------------------------------------------------------------
	MouseEvent(const Point2D &p, int buttons, Type type);
	//-------------------------------------------------------------------------
	MouseEvent(AComponentPtr source, const Point2D &p, int buttons, Type type);
	//-------------------------------------------------------------------------
	const Point2D & getLocation() const;
	//-------------------------------------------------------------------------
	const Point2D & getLocationOnScreen() const;
	//-------------------------------------------------------------------------
	Type getType() const;
	//-------------------------------------------------------------------------
	int getButtons() const;
	//-------------------------------------------------------------------------
	/**
	 * negative values if the mouse wheel was rotated up/away from the user, 
	 * and positive values if the mouse wheel was rotated down/ towards the user
	 */
	float getWheelRotation() const;
	//-------------------------------------------------------------------------
	AComponentPtr getSource() const;
	//-------------------------------------------------------------------------
	virtual std::string paramString() const;
	//-------------------------------------------------------------------------
	virtual std::string toString() const;
	//-------------------------------------------------------------------------
	// Update methods:
	MouseEvent & updateLocation(const Point2D &newLocation) {
		p = newLocation;
		return *this;
	}
	//-------------------------------------------------------------------------
	MouseEvent & updateSource(AComponentPtr _new) {
		source = _new;
		return *this;
	}
	//-------------------------------------------------------------------------
	MouseEvent & updateType(Type _new) {
		type = _new;
		return *this;
	}
	//-------------------------------------------------------------------------
	MouseEvent & updateButtons(int _new) {
		buttons = _new;
		return *this;
	}
	//-------------------------------------------------------------------------
	MouseEvent & updateWheelRotation(float _new) {
		wheelRotation = _new;
		return *this;
	}
};
///////////////////////////////////////////////////////////////////////////////
namespace {
//-----------------------------------------------------------------------------
template <int V>
struct Int2Type { enum { Value = V }; };
//-----------------------------------------------------------------------------
typedef MouseEvent Me;
template <class L>
void pressed(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void pressed(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_PRESSED> d) {
	l.mousePressed(ev);
}
template <class L>
void released(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void released(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_RELEASED> d) {
	l.mouseReleased(ev);
}
template <class L>
void entered(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void entered(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_ENTERED> d) {
	l.mouseEntered(ev);
}
template <class L>
void exited(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void exited(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_EXITED> d) {
	l.mouseExited(ev);
}
template <class L>
void clicked(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void clicked(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_CLICKED> d) {
	l.mouseClicked(ev);
}
template <class L>
void moved(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void moved(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_MOVED> d) {
	l.mouseMoved(ev);
}
template <class L>
void dragged(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void dragged(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_DRAGGED> d) {
	l.mouseDragged(ev);
}
template <class L>
void wheel(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void wheel(const MouseEvent &ev, L &l, Int2Type<Me::DISCO_MOUSE_WHEEL> d) {
	l.mouseWheelRotated(ev);
}
} // namespace
//=============================================================================
/**
 * Helper class for MouseEvent.
 * Delegates mouse events to fitting MouseEventListener methods.
 *
 */
template<int FilterValue = Me::ALL_EVENTS>
struct MouseEventSwitch {
//=============================================================================
	enum { Filter = FilterValue };
 	template <class MouseEventListener>
	static void delegate(const MouseEvent &ev, MouseEventListener &l) {
		switch (ev.getType()) {
		case MouseEvent::DISCO_MOUSE_PRESSED:
			pressed(ev, l, Int2Type<Me::DISCO_MOUSE_PRESSED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_RELEASED:
			released(ev, l, Int2Type<Me::DISCO_MOUSE_RELEASED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_CLICKED:
			clicked(ev, l, Int2Type<Me::DISCO_MOUSE_CLICKED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_ENTERED:
			entered(ev, l, Int2Type<Me::DISCO_MOUSE_ENTERED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_EXITED:
			exited(ev, l, Int2Type<Me::DISCO_MOUSE_EXITED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_MOVED:
			moved(ev, l, Int2Type<Me::DISCO_MOUSE_MOVED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_DRAGGED:
			dragged(ev, l, Int2Type<Me::DISCO_MOUSE_DRAGGED&Filter>());
			break;
		case MouseEvent::DISCO_MOUSE_WHEEL:
			wheel(ev, l, Int2Type<Me::DISCO_MOUSE_WHEEL&Filter>());
			break;
		default:
			break;
		}
	}
};
}}}} // namespace(s)

#endif /* SAMBAG_MOUSEEVENT_H */
