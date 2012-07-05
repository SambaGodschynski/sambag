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
namespace sambag { namespace disco { namespace components { namespace events {

//=============================================================================
/** 
  * @class MouseEvent.
  */
class MouseEvent {
friend class MouseEventCreator;
//=============================================================================
public:
	//-------------------------------------------------------------------------
	// Indicates mouse button #1; used by getButton().
	static const int	BUTTON1 = 1;
	// Indicates mouse button #2; used by getButton().
	static const int	BUTTON2 = 1<<1;
	// Indicates mouse button #3; used by getButton().
	static const int	BUTTON3 = 1<<2;
	enum Type {
		NONE,
		MOUSE_CLICKED = 1,
		MOUSE_DRAGGED = 1 << 1,
		MOUSE_ENTERED = 1 << 2,
		MOUSE_EXITED = 1 << 3,
		MOUSE_MOVED = 1 << 4,
		MOUSE_PRESSED = 1 << 5,
		MOUSE_RELEASED = 1 << 6,
		MOUSE_WHEEL = 1 << 7,
		ALL_EVENTS = MOUSE_PRESSED | MOUSE_RELEASED | MOUSE_CLICKED |
		MOUSE_MOVED | MOUSE_DRAGGED | MOUSE_ENTERED | MOUSE_EXITED | MOUSE_WHEEL
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
	Type type;
public:
	//-------------------------------------------------------------------------
	MouseEvent() : buttons(0), type(NONE) {};
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
	MouseEvent & updateSoure(AComponentPtr _new) {
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
void pressed(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_PRESSED> d) {
	l.mousePressed(ev);
}
template <class L>
void released(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void released(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_RELEASED> d) {
	l.mouseReleased(ev);
}
template <class L>
void entered(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void entered(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_ENTERED> d) {
	l.mouseEntered(ev);
}
template <class L>
void exited(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void exited(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_EXITED> d) {
	l.mouseExited(ev);
}
template <class L>
void clicked(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void clicked(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_CLICKED> d) {
	l.mouseClicked(ev);
}
template <class L>
void moved(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void moved(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_MOVED> d) {
	l.mouseMoved(ev);
}
template <class L>
void dragged(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void dragged(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_DRAGGED> d) {
	l.mouseDragged(ev);
}
template <class L>
void wheel(const MouseEvent &ev, L &l, Int2Type<0> d){}
template <class L>
void wheel(const MouseEvent &ev, L &l, Int2Type<Me::MOUSE_WHEEL> d) {
	l.mouseWheelMoved(ev);
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
		case MouseEvent::MOUSE_PRESSED:
			pressed(ev, l, Int2Type<Me::MOUSE_PRESSED&Filter>());
			break;
		case MouseEvent::MOUSE_RELEASED:
			released(ev, l, Int2Type<Me::MOUSE_RELEASED&Filter>());
			break;
		case MouseEvent::MOUSE_CLICKED:
			clicked(ev, l, Int2Type<Me::MOUSE_CLICKED&Filter>());
			break;
		case MouseEvent::MOUSE_ENTERED:
			entered(ev, l, Int2Type<Me::MOUSE_ENTERED&Filter>());
			break;
		case MouseEvent::MOUSE_EXITED:
			exited(ev, l, Int2Type<Me::MOUSE_EXITED&Filter>());
			break;
		case MouseEvent::MOUSE_MOVED:
			moved(ev, l, Int2Type<Me::MOUSE_MOVED&Filter>());
			break;
		case MouseEvent::MOUSE_DRAGGED:
			dragged(ev, l, Int2Type<Me::MOUSE_DRAGGED&Filter>());
			break;
		case MouseEvent::MOUSE_WHEEL:
			wheel(ev, l, Int2Type<Me::MOUSE_WHEEL&Filter>());
			break;
		default:
			break;
		}
	}
};
}}}} // namespace(s)

#endif /* SAMBAG_MOUSEEVENT_H */
