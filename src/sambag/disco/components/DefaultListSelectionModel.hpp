/*
 * DefaultListSelectionModel.hpp
 *
 *  Created on: Wed Sep  5 20:54:08 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTLISTSELECTIONMODEL_H
#define SAMBAG_DEFAULTLISTSELECTIONMODEL_H

#include <boost/shared_ptr.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <limits.h>
#include <boost/dynamic_bitset.hpp>
#include <sambag/com/events/Events.hpp>
#include "events/ListSelectionEvent.hpp"

namespace sambag { namespace disco { namespace components {
namespace ev = sambag::com::events;
//=============================================================================
/** 
  * @class DefaultListSelectionModel.
  */
class DefaultListSelectionModel :
	public ev::EventSender<events::ListSelectionEvent>
{
//=============================================================================
public:
	enum SelectionMode {
		//A value for the selectionMode property: select one or more
		//contiguous ranges of indices at a time.
		MULTIPLE_INTERVAL_SELECTION,
		//A value for the selectionMode property: select one contiguous
		//range of indices at a time.
		SINGLE_INTERVAL_SELECTION,
		//A value for the selectionMode property: select one
		//list index at a time.
		SINGLE_SELECTION
	};
protected:
private:
public:
	//-------------------------------------------------------------------------
	typedef events::ListSelectionEvent Event;
private:
	//-------------------------------------------------------------------------
	static const int MIN = -1;
	static const int MAX = INT_MAX;
	SelectionMode selectionMode;
	int minIndex;
	int maxIndex;
	int anchorIndex;
	int leadIndex;
	int firstAdjustedIndex;
	int lastAdjustedIndex;
	bool isAdjusting;
	int firstChangedIndex;
	int lastChangedIndex;
	typedef boost::dynamic_bitset<> BitSet;

protected:
	//-------------------------------------------------------------------------
	bool leadAnchorNotificationEnabled;
private:
	//-------------------------------------------------------------------------
	BitSet value;
public:
	//-------------------------------------------------------------------------
	int getMinSelectionIndex() const {
		return isSelectionEmpty() ? -1 : minIndex;
	}
	//-------------------------------------------------------------------------
	int getMaxSelectionIndex() const {
		return maxIndex;
	}
	//-------------------------------------------------------------------------
	bool getValueIsAdjusting() const {
		return isAdjusting;
	}
	//-------------------------------------------------------------------------
	SelectionMode getSelectionMode() const {
		return selectionMode;
	}
	//-------------------------------------------------------------------------
	void setSelectionMode(SelectionMode mode);
	//-------------------------------------------------------------------------
	bool isSelectedIndex(int index);
	//-------------------------------------------------------------------------
	bool isSelectionEmpty() const;
	//-------------------------------------------------------------------------
	void fireValueChanged(bool isAdjusting);
	//-------------------------------------------------------------------------
private:
	/**
	 * @param firstIndex the first index in the interval
	 * @param lastIndex the last index in the interval
	 * @param isAdjusting true if this is the final change in a series of
	 *          adjustments
	 * @see EventListenerList
	 */
	//-------------------------------------------------------------------------
	void fireValueChanged(int firstIndex, int lastIndex);
	//-------------------------------------------------------------------------
	void fireValueChanged(int firstIndex, int lastIndex, bool isAdjusting);
	//-------------------------------------------------------------------------
	void fireValueChanged();
	//-------------------------------------------------------------------------
	// Updates first and last change indices
	void markAsDirty(int r);
	//-------------------------------------------------------------------------
	// Sets the state at this index and update all relevant state.
	void setState(int r);
	// Clears the state at this index and update all relevant state.
	void clearState(int r);
	//-------------------------------------------------------------------------
	void updateLeadAnchorIndices(int anchorIndex, int leadIndex);
	//-------------------------------------------------------------------------
	bool contains(int a, int b, int i);
	//-------------------------------------------------------------------------
	void changeSelection(int clearMin, int clearMax, int setMin, int setMax,
			bool clearFirst);
	//-------------------------------------------------------------------------
	/**
	 * Change the selection with the effect of first clearing the values
	 * in the inclusive range [clearMin, clearMax] then setting the values
	 * in the inclusive range [setMin, setMax]. Do this in one pass so
	 * that no values are cleared if they would later be set.
	 */
	void changeSelection(int clearMin, int clearMax, int setMin, int setMax);
	//-------------------------------------------------------------------------
	void removeSelectionIntervalImpl(int index0, int index1,
			bool changeLeadAnchor);
	//-------------------------------------------------------------------------
	void setState(int index, bool state);
	//-------------------------------------------------------------------------
	/**
	 * Insert length indices beginning before/after index. If the value
	 * at index is itself selected and the selection mode is not
	 * SINGLE_SELECTION, set all of the newly inserted items as selected.
	 * Otherwise leave them unselected. This method is typically
	 * called to sync the selection model with a corresponding change
	 * in the data model.
	 */
public:
	//-------------------------------------------------------------------------
	DefaultListSelectionModel() :
		selectionMode(MULTIPLE_INTERVAL_SELECTION),
		minIndex(MAX),
		maxIndex(MIN),
		anchorIndex(-1),
		leadIndex(-1),
		firstAdjustedIndex(MAX),
		lastAdjustedIndex(MIN),
		isAdjusting(false),
		firstChangedIndex(MAX),
		lastChangedIndex(MIN),
		leadAnchorNotificationEnabled(true),
		value(32) // 32 bits
	{
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the value of the leadAnchorNotificationEnabled flag.
	 * @see             #isLeadAnchorNotificationEnabled()
	 */
	//-------------------------------------------------------------------------
	void setLeadAnchorNotificationEnabled(bool flag) {
		leadAnchorNotificationEnabled = flag;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the value of the <code>leadAnchorNotificationEnabled</code> flag.
	 * When <code>leadAnchorNotificationEnabled</code> is true the model
	 * generates notification events with bounds that cover all the changes to
	 * the selection plus the changes to the lead and anchor indices.
	 * Setting the flag to false causes a narrowing of the event's bounds to
	 * include only the elements that have been selected or deselected since
	 * the last change. Either way, the model continues to maintain the lead
	 * and anchor variables internally. The default is true.
	 * <p>
	 * Note: It is possible for the lead or anchor to be changed without a
	 * change to the selection. Notification of these changes is often
	 * important, such as when the new lead or anchor needs to be updated in
	 * the view. Therefore, caution is urged when changing the default value.
	 *
	 * @return  the value of the <code>leadAnchorNotificationEnabled</code> flag
	 * @see             #setLeadAnchorNotificationEnabled(bool)
	 */
	bool isLeadAnchorNotificationEnabled() const;
	//-------------------------------------------------------------------------
	void clearSelection();
	//-------------------------------------------------------------------------
	/**
	 * Changes the selection to be between {@code index0} and {@code index1}
	 * inclusive. {@code index0} doesn't have to be less than or equal to
	 * {@code index1}.
	 * <p>
	 * In {@code SINGLE_SELECTION} selection mode, only the second index
	 * is used.
	 * <p>
	 * If this represents a change to the current selection, then each
	 * {@code ListSelectionListener} is notified of the change.
	 * <p>
	 * If either index is {@code -1}, this method does nothing and returns
	 * without exception. Otherwise, if either index is less than {@code -1},
	 * an {@code IndexOutOfBoundsException} is thrown.
	 *
	 * @param index0 one end of the interval.
	 * @param index1 other end of the interval
	 * @throws IndexOutOfBoundsException if either index is less than {@code -1}
	 *         (and neither index is {@code -1})
	 * @see #addListSelectionListener
	 */
	void setSelectionInterval(int index0, int index1);
	//-------------------------------------------------------------------------
	/**
	 * Changes the selection to be the set union of the current selection
	 * and the indices between {@code index0} and {@code index1} inclusive.
	 * <p>
	 * In {@code SINGLE_SELECTION} selection mode, this is equivalent
	 * to calling {@code setSelectionInterval}, and only the second index
	 * is used. In {@code SINGLE_INTERVAL_SELECTION} selection mode, this
	 * method behaves like {@code setSelectionInterval}, unless the given
	 * interval is immediately adjacent to or overlaps the existing selection,
	 * and can therefore be used to grow it.
	 * <p>
	 * If this represents a change to the current selection, then each
	 * {@code ListSelectionListener} is notified of the change. Note that
	 * {@code index0} doesn't have to be less than or equal to {@code index1}.
	 * <p>
	 * If either index is {@code -1}, this method does nothing and returns
	 * without exception. Otherwise, if either index is less than {@code -1},
	 * an {@code IndexOutOfBoundsException} is thrown.
	 *
	 * @param index0 one end of the interval.
	 * @param index1 other end of the interval
	 * @throws IndexOutOfBoundsException if either index is less than {@code -1}
	 *         (and neither index is {@code -1})
	 * @see #addListSelectionListener
	 * @see #setSelectionInterval
	 */
	void addSelectionInterval(int index0, int index1);
	//-------------------------------------------------------------------------
	/**
	 * Changes the selection to be the set difference of the current selection
	 * and the indices between {@code index0} and {@code index1} inclusive.
	 * {@code index0} doesn't have to be less than or equal to {@code index1}.
	 * <p>
	 * In {@code SINGLE_INTERVAL_SELECTION} selection mode, if the removal
	 * would produce two disjoint selections, the removal is extended through
	 * the greater end of the selection. For example, if the selection is
	 * {@code 0-10} and you supply indices {@code 5,6} (in any order) the
	 * resulting selection is {@code 0-4}.
	 * <p>
	 * If this represents a change to the current selection, then each
	 * {@code ListSelectionListener} is notified of the change.
	 * <p>
	 * If either index is {@code -1}, this method does nothing and returns
	 * without exception. Otherwise, if either index is less than {@code -1},
	 * an {@code IndexOutOfBoundsException} is thrown.
	 *
	 * @param index0 one end of the interval
	 * @param index1 other end of the interval
	 * @throws IndexOutOfBoundsException if either index is less than {@code -1}
	 *         (and neither index is {@code -1})
	 * @see #addListSelectionListener
	 */
	void removeSelectionInterval(int index0, int index1);
	//-------------------------------------------------------------------------
	void insertIndexInterval(int index, int length, bool before);
	//-------------------------------------------------------------------------
	/**
	 * Remove the indices in the interval index0,index1 (inclusive) from
	 * the selection model.  This is typically called to sync the selection
	 * model width a corresponding change in the data model.  Note
	 * that (as always) index0 need not be <= index1.
	 */
	void removeIndexInterval(int index0, int index1);
	//-------------------------------------------------------------------------
	void setValueIsAdjusting(bool isAdjusting);
	//-------------------------------------------------------------------------
	int getAnchorSelectionIndex() const;
	//-------------------------------------------------------------------------
	int getLeadSelectionIndex() const;
	//-------------------------------------------------------------------------
	/**
	 * Set the anchor selection index, leaving all selection values unchanged.
	 * If leadAnchorNotificationEnabled is true, send a notification covering
	 * the old and new anchor cells.
	 *
	 * @see #getAnchorSelectionIndex
	 * @see #setLeadSelectionIndex
	 */
	void setAnchorSelectionIndex(int anchorIndex);
	//-------------------------------------------------------------------------
	/**
	 * Set the lead selection index, leaving all selection values unchanged.
	 * If leadAnchorNotificationEnabled is true, send a notification covering
	 * the old and new lead cells.
	 *
	 * @param leadIndex the new lead selection index
	 *
	 * @see #setAnchorSelectionIndex
	 * @see #setLeadSelectionIndex
	 * @see #getLeadSelectionIndex
	 *
	 * @since 1.5
	 */
	void moveLeadSelectionIndex(int leadIndex);
	//-------------------------------------------------------------------------
	/**
	 * Sets the lead selection index, ensuring that values between the
	 * anchor and the new lead are either all selected or all deselected.
	 * If the value at the anchor index is selected, first clear all the
	 * values in the range [anchor, oldLeadIndex], then select all the values
	 * values in the range [anchor, newLeadIndex], where oldLeadIndex is the old
	 * leadIndex and newLeadIndex is the new one.
	 * <p>
	 * If the value at the anchor index is not selected, do the same thing in
	 * reverse selecting values in the old range and deslecting values in the
	 * new one.
	 * <p>
	 * Generate a single event for this change and notify all listeners.
	 * For the purposes of generating minimal bounds in this event, do the
	 * operation in a single pass; that way the first and last index inside the
	 * ListSelectionEvent that is broadcast will refer to cells that actually
	 * changed value because of this method. If, instead, this operation were
	 * done in two steps the effect on the selection state would be the same
	 * but two events would be generated and the bounds around the changed
	 * values would be wider, including cells that had been first cleared only
	 * to later be set.
	 * <p>
	 * This method can be used in the <code>mouseDragged</code> method
	 * of a UI class to extend a selection.
	 *
	 * @see #getLeadSelectionIndex
	 * @see #setAnchorSelectionIndex
	 */
	void setLeadSelectionIndex(int leadIndex);
}; // DefaultListSelectionModel
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTLISTSELECTIONMODEL_H */
