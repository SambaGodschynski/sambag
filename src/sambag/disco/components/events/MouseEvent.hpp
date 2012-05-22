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
	static const int	BUTTON2 = 1<<2;
	// Indicates mouse button #3; used by getButton().
	static const int	BUTTON3 = 1<<3;
	enum Type {
		MOUSE_CLICKED,
		MOUSE_DRAGGED,
		MOUSE_ENTERED,
		MOUSE_EXITED,
		MOUSE_MOVED,
		MOUSE_PRESSED,
		MOUSE_RELEASED,
		MOUSE_WHEEL,
		NONE
	};
private:
	//-------------------------------------------------------------------------
	AComponentPtr source;
	//-------------------------------------------------------------------------
	Point2D p;
	//-------------------------------------------------------------------------
	int buttons;
	//-------------------------------------------------------------------------
	Type type;
public:
	//-------------------------------------------------------------------------
	MouseEvent() : buttons(0), type(NONE) {};
	//-------------------------------------------------------------------------
	MouseEvent(AComponentPtr source, const MouseEvent &ev);
	//-------------------------------------------------------------------------
	MouseEvent(const Point2D &p, int buttons, Type type);
	//-------------------------------------------------------------------------
	MouseEvent(AComponentPtr source, const Point2D &p, int buttons, Type type);
	//-------------------------------------------------------------------------
	const Point2D & getLocation() const;
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
};
}}}} // namespace(s)

#endif /* SAMBAG_MOUSEEVENT_H */
