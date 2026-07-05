/*
 * DiscoEvent.hpp
 *
 *  Created on: Mon May 27 13:36:46 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DISCOEVENT_H
#define SAMBAG_DISCOEVENT_H

#include <memory>

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
/** 
  * @class DiscoEvent.
  * Base class for all Disco events.
  */
class DiscoEvent {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::shared_ptr<DiscoEvent> Ptr;
protected:
	//-------------------------------------------------------------------------
	DiscoEvent();
private:
	//-------------------------------------------------------------------------
	mutable bool consumed;
public:
	//-------------------------------------------------------------------------
	bool isConsumed() const { return consumed; }
	//-------------------------------------------------------------------------
	void consume() const;
}; // DiscoEvent
}}}} // namespace(s)

#endif /* SAMBAG_DISCOEVENT_H */
