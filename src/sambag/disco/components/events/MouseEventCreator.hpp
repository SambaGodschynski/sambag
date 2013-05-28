/*
 * MouseEventCreator.hpp
 *
 *  Created on: Thu May 31 14:06:51 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MOUSEEVENTCREATOR_H
#define SAMBAG_MOUSEEVENTCREATOR_H

#include <boost/shared_ptr.hpp>
#include "MouseEvent.hpp"
#include <sambag/com/events/Events.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/components/Forward.hpp>

namespace sambag { namespace disco { namespace components { namespace events {
namespace sce = sambag::com::events;
//=============================================================================
/** 
  * @class MouseEventCreator.
  * Helperclass for creating MouseEvents.
  */
class MouseEventCreator : public sce::EventSender <MouseEvent> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef int Bitmask;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<MouseEventCreator> Ptr;
protected:
	//-------------------------------------------------------------------------
	MouseEvent lastEvent;
	//-------------------------------------------------------------------------
	RootPanePtr root;
	//-------------------------------------------------------------------------
	MouseEventCreator(RootPanePtr root) : root(root) {}
	//-------------------------------------------------------------------------
	void fireEvent(MouseEvent &ev);
public:
	//-------------------------------------------------------------------------
	static Ptr create(RootPanePtr root) {
		Ptr res(new MouseEventCreator(root));
		return res;
	}
	//-------------------------------------------------------------------------
	void
	createPressEvent(const Coordinate &x,
			const Coordinate &y, Bitmask buttons);
	//-------------------------------------------------------------------------
	void
	createReleaseEvent(const Coordinate &x,
			const Coordinate &y, Bitmask buttons);
	//-------------------------------------------------------------------------
	void
	createMoveEvent(const Coordinate &x, const Coordinate &y);
	//-------------------------------------------------------------------------
	void 
	createWheelEvent(const Coordinate &x, const Coordinate &y, float wheelRotation);
}; // MouseEventCreator
}}}} // namespace(s)

#endif /* SAMBAG_MOUSEEVENTCREATOR_H */
