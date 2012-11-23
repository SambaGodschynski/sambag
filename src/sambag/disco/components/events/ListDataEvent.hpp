/*
 * ListDataEvent.hpp
 *
 *  Created on: Wed Sep  5 15:52:52 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LISTDATAEVENT_H
#define SAMBAG_LISTDATAEVENT_H

#include <boost/shared_ptr.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>

namespace sambag { namespace disco {
namespace components { namespace events {
//=============================================================================
/** 
  * @class ListDataEvent.
  */
class ListDataEvent {
//=============================================================================
public:
	enum Type {
		//Identifies one or more changes in the lists contents.
		CONTENTS_CHANGED,
		//Identifies the addition of one or more contiguous items to the list
		INTERVAL_ADDED,
		//Identifies the removal of one or more contiguous items from the list
		INTERVAL_REMOVED
	};
private:
	Type type;
	sambag::com::ArithmeticWrapper<int> i0, i1;
public:
	//-------------------------------------------------------------------------
	ListDataEvent(Type type, int i0, int i1) : type(type), i0(i0), i1(i1) {}
	//-------------------------------------------------------------------------
	/**
	 * @return the lower index of the range.
	 */
	int getIndex0() const {
		return i0;
	}
	//-------------------------------------------------------------------------
	/**
	 * @return the upper index of the range.
	 */
	int getIndex1() const {
		return i1;
	}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @return the event type.
	 */
	Type getType() const {
		return type;
	}
}; // ListDataEvent
}}}} // namespace(s)

#endif /* SAMBAG_LISTDATAEVENT_H */
