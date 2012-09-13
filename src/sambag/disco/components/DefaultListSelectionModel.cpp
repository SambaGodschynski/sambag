/*
 * DefaultListSelectionModel.cpp
 *
 *  Created on: Wed Sep  5 20:54:08 2012
 *      Author: Johannes Unger
 */

#include "DefaultListSelectionModel.hpp"
#include <sambag/com/Common.hpp>
#include <algorithm>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class DefaultListSelectionModel
//=============================================================================
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::
setSelectionMode(DefaultListSelectionModel::SelectionMode mode)
{
	selectionMode = mode;
}
//-----------------------------------------------------------------------------
bool DefaultListSelectionModel::isSelectedIndex(int index) {
	if (index > value.size())
		return false;
	return
		((index < minIndex) || (index > maxIndex)) ? false : value[index];
}
//-----------------------------------------------------------------------------
bool DefaultListSelectionModel::isSelectionEmpty() const {
	return (minIndex > maxIndex);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::fireValueChanged(int firstIndex,
	int lastIndex)
{
	fireValueChanged(firstIndex, lastIndex, getValueIsAdjusting());
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::fireValueChanged() {
	if (lastAdjustedIndex == MIN) {
		return;
	}
	/* If getValueAdjusting() is true, (eg. during a drag opereration)
	 * record the bounds of the changes so that, when the drag finishes (and
	 * setValueAdjusting(false) is called) we can post a single event
	 * with bounds covering all of these individual adjustments.
	 */
	if (getValueIsAdjusting()) {
		firstChangedIndex = std::min(firstChangedIndex, firstAdjustedIndex);
		lastChangedIndex = std::max(lastChangedIndex, lastAdjustedIndex);
	}
	/* Change the values before sending the event to the
	 * listeners in case the event causes a listener to make
	 * another change to the selection.
	 */
	int oldFirstAdjustedIndex = firstAdjustedIndex;
	int oldLastAdjustedIndex = lastAdjustedIndex;
	firstAdjustedIndex = MAX;
	lastAdjustedIndex = MIN;

	fireValueChanged(oldFirstAdjustedIndex, oldLastAdjustedIndex);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::fireValueChanged(bool isAdjusting) {
	if (lastChangedIndex == MIN) {
		return;
	}
	/* Change the values before sending the event to the
	 * listeners in case the event causes a listener to make
	 * another change to the selection.
	 */
	int oldFirstChangedIndex = firstChangedIndex;
	int oldLastChangedIndex = lastChangedIndex;
	firstChangedIndex = MAX;
	lastChangedIndex = MIN;
	fireValueChanged(oldFirstChangedIndex, oldLastChangedIndex, isAdjusting);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::fireValueChanged(int firstIndex,
	int lastIndex, bool isAdjusting)
{
	ev::EventSender<events::ListSelectionEvent>::notifyListeners(
		this,
		events::ListSelectionEvent(firstIndex, lastIndex, isAdjusting)
	);
}
//-----------------------------------------------------------------------------
// Updates first and last change indices
void DefaultListSelectionModel::markAsDirty(int r)  {
	firstAdjustedIndex = std::min(firstAdjustedIndex, r);
	lastAdjustedIndex =  std::max(lastAdjustedIndex, r);
}
//-----------------------------------------------------------------------------
// Sets the state at this index and update all relevant state.
void DefaultListSelectionModel::setState(int r) {
	if (r > value.size())
		value.resize(r+1, false);
	if (value[r]) {
		return;
	}
	value[r] = true;
	markAsDirty(r);

	// Update minimum and maximum indices
	minIndex = std::min(minIndex, r);
	maxIndex = std::max(maxIndex, r);
}
//-----------------------------------------------------------------------------
// Clears the state at this index and update all relevant state.
void DefaultListSelectionModel::clearState(int r)  {
	if (r > value.size())
		return;
	if (!value[r]) {
		return;
	}
	value[r] = false;
	markAsDirty(r);

	// Update minimum and maximum indices
	/*
	 If (r > minIndex) the minimum has not changed.
	 The case (r < minIndex) is not possible because r'th value was set.
	 We only need to check for the case when lowest entry has been cleared,
	 and in this case we need to search for the first value set above it.
	 */
	if (r == minIndex) {
		for (minIndex = minIndex + 1; minIndex <= maxIndex; minIndex++) {
			if (value[minIndex]) {
				break;
			}
		}
	}
	/*
	 If (r < maxIndex) the maximum has not changed.
	 The case (r > maxIndex) is not possible because r'th value was set.
	 We only need to check for the case when highest entry has been cleared,
	 and in this case we need to search for the first value set below it.
	 */
	if (r == maxIndex) {
		for (maxIndex = maxIndex - 1; minIndex <= maxIndex; maxIndex--) {
			if (value[maxIndex]) {
				break;
			}
		}
	}
	/* Performance note: This method is called from inside a loop in
	 changeSelection() but we will only iterate in the loops
	 above on the basis of one iteration per deselected cell - in total.
	 Ie. the next time this method is called the work of the previous
	 deselection will not be repeated.

	 We also don't need to worry about the case when the min and max
	 values are in their unassigned states. This cannot happen because
	 this method's initial check ensures that the selection was not empty
	 and therefore that the minIndex and maxIndex had 'real' values.

	 If we have cleared the whole selection, set the minIndex and maxIndex
	 to their cannonical values so that the next set command always works
	 just by using Math.min and Math.max.
	 */
	if (isSelectionEmpty()) {
		minIndex = MAX;
		maxIndex = MIN;
	}
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::updateLeadAnchorIndices(int anchorIndex,
		int leadIndex)
{
	if (leadAnchorNotificationEnabled) {
		if (this->anchorIndex != anchorIndex) {
			if (this->anchorIndex != -1) { // The unassigned state.
				markAsDirty(this->anchorIndex);
			}
			markAsDirty(anchorIndex);
		}
		if (this->leadIndex != leadIndex) {
			if (this->leadIndex != -1) { // The unassigned state.
				markAsDirty(this->leadIndex);
			}
			markAsDirty(leadIndex);
		}
	}
	this->anchorIndex = anchorIndex;
	this->leadIndex = leadIndex;
}
//-----------------------------------------------------------------------------
bool DefaultListSelectionModel::contains(int a, int b, int i)  {
	return (i >= a) && (i <= b);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::changeSelection(int clearMin, int clearMax,
		int setMin, int setMax,
		bool clearFirst)
{
	for (int i = std::min(setMin, clearMin);
			i <= std::max(setMax, clearMax); i++)
	{
		bool shouldClear = contains(clearMin, clearMax, i);
		bool shouldSet = contains(setMin, setMax, i);

		if (shouldSet && shouldClear) {
			if (clearFirst) {
				shouldClear = false;
			} else {
				shouldSet = false;
			}
		}
		if (shouldSet) {
			setState(i);
		}
		if (shouldClear) {
			clearState(i);
		}
	}
	fireValueChanged();
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::changeSelection(int clearMin, int clearMax,
		int setMin, int setMax)
{
	 changeSelection(clearMin, clearMax, setMin, setMax, true);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::removeSelectionIntervalImpl(int index0,
		int index1, bool changeLeadAnchor)
{
	if (index0 == -1 || index1 == -1) {
		return;
	}

	if (changeLeadAnchor) {
		updateLeadAnchorIndices(index0, index1);
	}

	int clearMin = std::min(index0, index1);
	int clearMax = std::max(index0, index1);
	int setMin = MAX;
	int setMax = MIN;

	// If the removal would produce to two disjoint selections in a mode
	// that only allows one, extend the removal to the end of the selection.
	if (getSelectionMode() != MULTIPLE_INTERVAL_SELECTION && clearMin
			> minIndex && clearMax < maxIndex) {
		clearMax = maxIndex;
	}

	changeSelection(clearMin, clearMax, setMin, setMax);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::setState(int index, bool state) {
	if (state) {
		setState(index);
	} else {
		clearState(index);
	}
}
//-----------------------------------------------------------------------------
bool DefaultListSelectionModel::isLeadAnchorNotificationEnabled() const
{
	return leadAnchorNotificationEnabled;
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::clearSelection() {
	removeSelectionIntervalImpl(minIndex, maxIndex, false);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::setSelectionInterval(int index0,
	int index1)
{
	if (index0 == -1 || index1 == -1) {
		return;
	}

	if (getSelectionMode() == SINGLE_SELECTION) {
		index0 = index1;
	}

	updateLeadAnchorIndices(index0, index1);

	int clearMin = minIndex;
	int clearMax = maxIndex;
	int setMin = std::min(index0, index1);
	int setMax = std::max(index0, index1);
	changeSelection(clearMin, clearMax, setMin, setMax);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::addSelectionInterval(int index0,
		int index1)
{
	if (index0 == -1 || index1 == -1) {
		return;
	}
	// If we only allow a single selection, channel through
	// setSelectionInterval() to enforce the rule.
	if (getSelectionMode() == SINGLE_SELECTION) {
		setSelectionInterval(index0, index1);
		return;
	}
	updateLeadAnchorIndices(index0, index1);

	int clearMin = MAX;
	int clearMax = MIN;
	int setMin = std::min(index0, index1);
	int setMax = std::max(index0, index1);

	// If we only allow a single interval and this would result
	// in multiple intervals, then set the selection to be just
	// the new range.
	if (getSelectionMode() == SINGLE_INTERVAL_SELECTION && (setMax < minIndex
			- 1 || setMin > maxIndex + 1)) {

		setSelectionInterval(index0, index1);
		return;
	}
	changeSelection(clearMin, clearMax, setMin, setMax);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::removeSelectionInterval(int index0,
	int index1)
{
	removeSelectionIntervalImpl(index0, index1, true);
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::insertIndexInterval(int index, int length,
	bool before)
{
	/* The first new index will appear at insMinIndex and the last
	 * one will appear at insMaxIndex
	 */
	int insMinIndex = (before) ? index : index + 1;
	int insMaxIndex = (insMinIndex + length) - 1;

	/* Right shift the entire bitset by length, beginning with
	 * index-1 if before is true, index+1 if it's false (i.e. with
	 * insMinIndex).
	 */
	for (int i = maxIndex; i >= insMinIndex; i--) {
		setState(i + length, value[i]);
	}

	/* Initialize the newly inserted indices.
	 */
	bool setInsertedValues =
			((getSelectionMode() == SINGLE_SELECTION) ? false
					: value[index]);
	for (int i = insMinIndex; i <= insMaxIndex; i++) {
		setState(i, setInsertedValues);
	}

	int leadIndex = this->leadIndex;
	if (leadIndex > index || (before && leadIndex == index)) {
		leadIndex = this->leadIndex + length;
	}
	int anchorIndex = this->anchorIndex;
	if (anchorIndex > index || (before && anchorIndex == index)) {
		anchorIndex = this->anchorIndex + length;
	}
	if (leadIndex != this->leadIndex || anchorIndex != this->anchorIndex) {
		updateLeadAnchorIndices(anchorIndex, leadIndex);
	}

	fireValueChanged();
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::removeIndexInterval(int index0, int index1)
{
	int rmMinIndex = std::min(index0, index1);
	int rmMaxIndex = std::max(index0, index1);
	int gapLength = (rmMaxIndex - rmMinIndex) + 1;

	/* Shift the entire bitset to the left to close the index0, index1
	 * gap.
	 */
	for (int i = rmMinIndex; i <= maxIndex; i++) {
		setState(i, value[i + gapLength]);
	}

	int leadIndex = this->leadIndex;
	if (leadIndex == 0 && rmMinIndex == 0) {
		// do nothing
	} else if (leadIndex > rmMaxIndex) {
		leadIndex = this->leadIndex - gapLength;
	} else if (leadIndex >= rmMinIndex) {
		leadIndex = rmMinIndex - 1;
	}

	int anchorIndex = this->anchorIndex;
	if (anchorIndex == 0 && rmMinIndex == 0) {
		// do nothing
	} else if (anchorIndex > rmMaxIndex) {
		anchorIndex = this->anchorIndex - gapLength;
	} else if (anchorIndex >= rmMinIndex) {
		anchorIndex = rmMinIndex - 1;
	}

	if (leadIndex != this->leadIndex || anchorIndex != this->anchorIndex) {
		updateLeadAnchorIndices(anchorIndex, leadIndex);
	}

	fireValueChanged();
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::setValueIsAdjusting(bool isAdjusting)
{
	if (isAdjusting != this->isAdjusting) {
		this->isAdjusting = isAdjusting;
		this->fireValueChanged(isAdjusting);
	}
}
//-----------------------------------------------------------------------------
int DefaultListSelectionModel::getAnchorSelectionIndex() const {
	return anchorIndex;
}
//-----------------------------------------------------------------------------
int DefaultListSelectionModel::getLeadSelectionIndex() const  {
	return leadIndex;
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::setAnchorSelectionIndex(int anchorIndex) {
	updateLeadAnchorIndices(anchorIndex, this->leadIndex);
	fireValueChanged();
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::moveLeadSelectionIndex(int leadIndex) {
	// disallow a -1 lead unless the anchor is already -1
	if (leadIndex == -1) {
		if (this->anchorIndex != -1) {
			return;
		}
		/* PENDING(shannonh) - The following check is nice, to be consistent with
		 setLeadSelectionIndex. However, it is not absolutely
		 necessary: One could work around it by setting the anchor
		 to something valid, modifying the lead, and then moving
		 the anchor back to -1. For this reason, there's no sense
		 in adding it at this time, as that would require
		 updating the spec and officially committing to it.

		 // otherwise, don't do anything if the anchor is -1
		 } else if (this.anchorIndex == -1) {
		 return;
		 */
	}
	updateLeadAnchorIndices(this->anchorIndex, leadIndex);
	fireValueChanged();
}
//-----------------------------------------------------------------------------
void DefaultListSelectionModel::setLeadSelectionIndex(int leadIndex) {
	int anchorIndex = this->anchorIndex;

	// only allow a -1 lead if the anchor is already -1
	if (leadIndex == -1) {
		if (anchorIndex == -1) {
			updateLeadAnchorIndices(anchorIndex, leadIndex);
			fireValueChanged();
		}

		return;
		// otherwise, don't do anything if the anchor is -1
	} else if (anchorIndex == -1) {
		return;
	}

	if (this->leadIndex == -1) {
		this->leadIndex = leadIndex;
	}

	bool shouldSelect = value[this->anchorIndex];

	if (getSelectionMode() == SINGLE_SELECTION) {
		anchorIndex = leadIndex;
		shouldSelect = true;
	}

	int oldMin = std::min(this->anchorIndex, this->leadIndex);
	int oldMax = std::max(this->anchorIndex, this->leadIndex);
	int newMin = std::min(anchorIndex, leadIndex);
	int newMax = std::max(anchorIndex, leadIndex);

	updateLeadAnchorIndices(anchorIndex, leadIndex);

	if (shouldSelect) {
		changeSelection(oldMin, oldMax, newMin, newMax);
	} else {
		changeSelection(newMin, newMax, oldMin, oldMax, false);
	}
}


}}} // namespace(s)
