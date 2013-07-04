/*
 * MouseEventRecorder.hpp
 *
 *  Created on: Wed Jul  3 21:35:17 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MOUSEEVENTRECORDER_H
#define SAMBAG_MOUSEEVENTRECORDER_H

#include <boost/shared_ptr.hpp>
#include "MouseEventCreator.hpp"
namespace sambag { namespace disco { namespace components { namespace events {
namespace sce = sambag::com::events;
//=============================================================================
/** 
  * @class MouseEventRecorder.
  * This class is a MouseEventCreator which is able to record 
  * and replay mouse events.
  * initalization:
  * getWindowToolkit()->setMouseEventCreatorPrototype(MouseEventRecorder::create());
  */
class MouseEventRecorder : public MouseEventCreator {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<MouseEventRecorder> Ptr;
    //-------------------------------------------------------------------------
    typedef MouseEventCreator Super;
protected:
private:
public:
    //-------------------------------------------------------------------------
    virtual Super::Ptr clone() const;
	//-------------------------------------------------------------------------
	static Ptr create();
	//-------------------------------------------------------------------------
	virtual void
	createPressEvent(const Coordinate &x,
                     const Coordinate &y, Bitmask buttons);
	//-------------------------------------------------------------------------
	virtual void
	createReleaseEvent(const Coordinate &x,
                       const Coordinate &y, Bitmask buttons);
	//-------------------------------------------------------------------------
	virtual void
	createMoveEvent(const Coordinate &x, const Coordinate &y);
	//-------------------------------------------------------------------------
	virtual void
	createWheelEvent(const Coordinate &x, const Coordinate &y, float wheelRotation);
}; // MouseEventRecorder
}}}} // namespace(s)

#endif /* SAMBAG_MOUSEEVENTRECORDER_H */
