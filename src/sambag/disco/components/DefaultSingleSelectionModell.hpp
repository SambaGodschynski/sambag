/*
 * DefaultSingleSelectionModell.hpp
 *
 *  Created on: Mon Jun 11 11:47:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTSINGLESELECTIONMODELL_H
#define SAMBAG_DEFAULTSINGLESELECTIONMODELL_H

#include <boost/shared_ptr.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/com/events/ChangedEvent.hpp>
#include <sambag/com/events/Events.hpp>

namespace sambag { namespace disco { namespace components {
namespace ev = sambag::com::events;
class DefaultSingleSelectionModell;
typedef ev::ChangedEvent<DefaultSingleSelectionModell>
	DefaultSingleSelectionModellChanged;
//=============================================================================
/** 
  * @class DefaultSingleSelectionModell.
  */
class DefaultSingleSelectionModell :
	public ev::EventSender<DefaultSingleSelectionModellChanged>
{
//=============================================================================
private:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int, -1> selection;
public:
	//-------------------------------------------------------------------------
	/**
	 * Clears the selection (to -1).
	 */
	void clearSelection();
	//-------------------------------------------------------------------------
	/**
	 * Returns the model's selection.
	 * @return
	 */
	int getSelectedIndex() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if the selection model currently has a selected value.
	 * @return
	 */
	bool isSelected() const;
	//-------------------------------------------------------------------------
	/**
	 * Sets the model's selected index to index.
	 * @param index
	 */
	void setSelectedIndex(int index);

}; // DefaultSingleSelectionModell
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTSINGLESELECTIONMODELL_H */
