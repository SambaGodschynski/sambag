/*
 * ListSelectionEvent.hpp
 *
 *  Created on: Thu Sep  6 11:28:17 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LISTSELECTIONEVENT_H
#define SAMBAG_LISTSELECTIONEVENT_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco { namespace components {
namespace events {
//=============================================================================
/** 
  * @class ListSelectionEvent.
  */
class ListSelectionEvent {
//=============================================================================
private:
	int fI, lI;
	bool isAdjusting;
public:
	//-------------------------------------------------------------------------
	ListSelectionEvent(int firstIndex, int lastIndex, bool isAdjusting) :
		fI(firstIndex), lI(lastIndex), isAdjusting(isAdjusting) {}
	//-------------------------------------------------------------------------
	/**
	 * Returns the index of the first row whose selection may have changed.
	 * @return
	 */
	int getFirstIndex() const { return fI; }
	//-------------------------------------------------------------------------
	/**
	 * Returns the index of the last row whose selection may have changed.
	 * @return
	 */
	int getLastIndex() const { return lI; }
	//-------------------------------------------------------------------------
	/**
	 * Returns whether or not this is one in a series of multiple events,
	 * here changes are still being made.
	 * @return
	 */
	bool getValueIsAdjusting() const { return isAdjusting; }
}; // ListSelectionEvent
}}}} // namespace(s)

#endif /* SAMBAG_LISTSELECTIONEVENT_H */
