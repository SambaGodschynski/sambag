/*
 * DefaultListModel.hpp
 *
 *  Created on: Wed Sep  5 12:02:52 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTLISTMODEL_H
#define SAMBAG_DEFAULTLISTMODEL_H

#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/foreach.hpp>
#include <sambag/com/Helper.hpp>
#include <sambag/com/events/Events.hpp>
#include "events/ListDataEvent.hpp"
#include <algorithm>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class DefaultListModel.
  * The abstract definition for the data model that provides
  * a <code>AList</code> with its contents.
  */
template <class T>
class DefaultListModel :
	public sambag::com::events::EventSender<events::ListDataEvent>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::vector<T> Vector;
protected:
private:
	//-------------------------------------------------------------------------
	Vector delegate;
protected:
	//-------------------------------------------------------------------------
	/**
	 * @param index0 one end of the new interval
	 * @param index1 the other end of the new interval
	 */
	void fireContentsChanged(int index0, int index1) {
		EventSender<events::ListDataEvent>::notifyListeners( this,
			events::ListDataEvent(events::ListDataEvent::CONTENTS_CHANGED,
				index0,
				index1
			)
		);
	}
	//-------------------------------------------------------------------------
	/**
	 * <code>AbstractListModel</code> subclasses must call this method
	 * <b>after</b>
	 * one or more elements are added to the model.  The new elements
	 * are specified by a closed interval index0, index1 -- the enpoints
	 * are included.  Note that
	 * index0 need not be less than or equal to index1.
	 *
	 * @param source the <code>ListModel</code> that changed, typically "this"
	 * @param index0 one end of the new interval
	 * @param index1 the other end of the new interval
	 * @see EventListenerList
	 * @see DefaultListModel
	 */
	void fireIntervalAdded(int index0, int index1) {
		EventSender<events::ListDataEvent>::notifyListeners( this,
			events::ListDataEvent(events::ListDataEvent::INTERVAL_ADDED,
				index0,
				index1
			)
		);
	}
	//-------------------------------------------------------------------------
	/**
	 * <code>AbstractListModel</code> subclasses must call this method
	 * <b>after</b> one or more elements are removed from the model.
	 * <code>index0</code> and <code>index1</code> are the end points
	 * of the interval that's been removed.  Note that <code>index0</code>
	 * need not be less than or equal to <code>index1</code>.
	 *
	 * @param source the <code>ListModel</code> that changed, typically "this"
	 * @param index0 one end of the removed interval,
	 *               including <code>index0</code>
	 * @param index1 the other end of the removed interval,
	 *               including <code>index1</code>
	 * @see EventListenerList
	 * @see DefaultListModel
	 */
	void fireIntervalRemoved(int index0, int index1) {
		EventSender<events::ListDataEvent>::notifyListeners( this,
			events::ListDataEvent(events::ListDataEvent::INTERVAL_REMOVED,
				index0,
				index1
			)
		);
	}
public:
	//-------------------------------------------------------------------------
	/**
	 * Returns the number of components in this list.
	 * <p>
	 * This method is identical to <code>size</code>, which implements the
	 * <code>AList</code> interface defined in the 1.2 Collections framework.
	 * This method exists in conjunction with <code>setSize</code> so that
	 * <code>size</code> is identifiable as a JavaBean property.
	 *
	 * @return  the number of components in this list
	 * @see #size()
	 */
	size_t getSize() const {
		return delegate.size();
	}
	//-------------------------------------------------------------------------
	/**
	 * Copies the components of this list into the specified array.
	 * The array must be big enough to hold all the objects in this list.
	 *
	 * @param   anArray   the array into which the components get copied
	 * @see Vector#copyInto(Object[])
	 */
	template <class STLContainer>
	void copyInto(STLContainer &cont) const {
		BOOST_FOREACH(const T &obj, delegate) {
			cont.push_back(obj);
		}
	}
	//-------------------------------------------------------------------------
	/**
	 * Trims the capacity of this list to be the list's current size.
	 *
	 * @see Vector#trimToSize()
	 */
	void trimToSize() {
		delegate.reserve(delegate.size());
	}
	//-------------------------------------------------------------------------
	/**
	 * Increases the capacity of this list, if necessary, to ensure
	 * that it can hold at least the number of components specified by
	 * the minimum capacity argument.
	 *
	 * @param   minCapacity   the desired minimum capacity
	 * @see Vector#ensureCapacity(int)
	 */
	void ensureCapacity(size_t minCapacity) {
		delegate.reserve(minCapacity);
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the current capacity of this list.
	 *
	 * @return  the current capacity
	 * @see Vector#capacity()
	 */
	size_t capacity() const {
		return delegate.capacity();
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the number of components in this list.
	 *
	 * @return  the number of components in this list
	 * @see Vector#size()
	 */
	size_t size() const {
		return delegate.size();
	}
	//-------------------------------------------------------------------------
	/**
	 * Tests whether this list has any components.
	 *
	 * @return  <code>true</code> if and only if this list has
	 *          no components, that is, its size is zero;
	 *          <code>false</code> otherwise
	 * @see Vector#isEmpty()
	 */
	bool isEmpty() const {
		return delegate.empty();
	}
	/**
	 * Tests whether the specified object is a component in this list.
	 *
	 * @param   elem   an object
	 * @return  <code>true</code> if the specified object
	 *          is the same as a component in this list
	 * @see Vector#contains(Object)
	 */
	bool containsElement(const T &elem) const {
		return indexOf(elem) != -1;
	}
	//-------------------------------------------------------------------------
	/**
	 * Searches for the first occurrence of <code>elem</code>.
	 *
	 * @param   elem   an object
	 * @return  the index of the first occurrence of the argument in this
	 *          list; returns <code>-1</code> if the object is not found
	 * @see Vector#indexOf(Object)
	 */
	int indexOf(const T &elem) const {
		return sambag::com::indexOf(delegate, elem);
	}
	//-------------------------------------------------------------------------
	/**
	 * Searches for the first occurrence of <code>elem</code>, beginning
	 * the search at <code>index</code>.
	 *
	 * @param   elem    an desired component
	 * @param   index   the index from which to begin searching
	 * @return  the index where the first occurrence of <code>elem</code>
	 *          is found after <code>index</code>; returns <code>-1</code>
	 *          if the <code>elem</code> is not found in the list
	 * @see Vector#indexOf(Object,int)
	 */
	int indexOf(const T &elem, size_t index) const {
		++index;
		if (index > delegate.size())
			return -1;
		int c = index;
		typename Vector::const_iterator it = delegate.begin();
		it+=index;
		for (; it != delegate.end(); ++it)
		{
			if (*it==elem)
				return c;
			++c;
		}
		return -1;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the index of the last occurrence of <code>elem</code>.
	 *
	 * @param   elem   the desired component
	 * @return  the index of the last occurrence of <code>elem</code>
	 *          in the list; returns <code>-1</code> if the object is not found
	 * @see Vector#lastIndexOf(Object)
	 */
	int lastIndexOf(const T &elem) const {
		int res = -1;
		int c = 0;
		BOOST_FOREACH(const T &obj, delegate) {
			if (obj==elem)
				res = c;
			++c;
		}
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * Searches backwards for <code>elem</code>, starting from the
	 * specified index, and returns an index to it.
	 *
	 * @param  elem    the desired component
	 * @param  index   the index to start searching from
	 * @return the index of the last occurrence of the <code>elem</code>
	 *          in this list at position less than <code>index</code>;
	 *          returns <code>-1</code> if the object is not found
	 * @see Vector#lastIndexOf(Object,int)
	 */
	int lastIndexOf(const T &elem, size_t index) const {
		++index;
		if (index > delegate.size())
			return -1;
		int res = -1;
		int c = index;
		typename Vector::const_iterator it = delegate.begin();
		it+=index;
		for (; it != delegate.end(); ++it)
		{
			if (*it==elem)
				res = c;
			++c;
		}
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the first component of this list.
	 * Throws a <code>NoSuchElementException</code> if this
	 * delegate has no components.
	 * @return     the first component of this list
	 * @see Vector#firstElement()
	 */
	const T & firstElement() const {
		return *(delegate.begin());
	}
	//-------------------------------------------------------------------------
	T & firstElement() {
		return *(delegate.begin());
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the last component of the list.
	 * Throws a <code>NoSuchElementException</code> if this delegate
	 * has no components.
	 *
	 * @return  the last component of the list
	 * @see Vector#lastElement()
	 */
	const T & lastElement() const {
		return *(--(delegate.end()));
	}
	//-------------------------------------------------------------------------
	T & lastElement() {
		return *(--(delegate.end()));
	}
	//-------------------------------------------------------------------------
	/**
	 * Removes the first (lowest-indexed) occurrence of the argument
	 * from this list.
	 *
	 * @param   obj   the component to be removed
	 * @return  <code>true</code> if the argument was a component of this
	 *          list; <code>false</code> otherwise
	 * @see Vector#removeElement(Object)
	 */
	bool removeElement(const T &element) {
		typename Vector::iterator it =
				std::find(delegate.begin(), delegate.end(), element);
		int index = std::distance(delegate.begin(), it);
		if (it==delegate.end())
			return false;
		delegate.erase(it);
		fireIntervalRemoved(index, index);
		return true;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns a string that displays and identifies this
	 * object's properties.
	 *
	 * @return a String representation of this object
	 */
	std::string toString() const {
		return "DefaultListModel";
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the element at the specified position in this list.
	 * <p>
	 * Throws an <code>ArrayIndexOutOfBoundsException</code>
	 * if the index is out of range
	 * (<code>index &lt; 0 || index &gt;= size()</code>).
	 *
	 * @param index index of element to return
	 */
	const T & get(size_t index) const {
		return delegate[index];
	}
	//-------------------------------------------------------------------------
	T & get(size_t index) {
		return delegate[index];
	}
	//-------------------------------------------------------------------------
	/**
	 * Replaces the element at the specified position in this list with the
	 * specified element.
	 * <p>
	 * Throws an <code>ArrayIndexOutOfBoundsException</code>
	 * if the index is out of range
	 * (<code>index &lt; 0 || index &gt;= size()</code>).
	 *
	 * @param index index of element to replace
	 * @param element element to be stored at the specified position
	 * @return the element previously at the specified position
	 */
	T set(size_t index, const T & element) {
		T old = delegate[index];
		delegate[index] = element;
		fireContentsChanged(index, index);
		return old;
	}
	//-------------------------------------------------------------------------
	/**
	 * Inserts the specified element at the specified position in this list.
	 * <p>
	 * Throws an <code>ArrayIndexOutOfBoundsException</code> if the
	 * index is out of range
	 * (<code>index &lt; 0 || index &gt; size()</code>).
	 *
	 * @param index index at which the specified element is to be inserted
	 * @param element element to be inserted
	 */
	void addElement(const T &element, int index = -1) {
		if (index == -1) {
			index = delegate.size();
			delegate.push_back(element);
			fireIntervalAdded(index, index);
			return;
		}
		typename Vector::iterator it = delegate.begin();
		it+=index;
		delegate.insert(it, element);
		fireIntervalAdded(index, index);
	}
	//-------------------------------------------------------------------------
	/**
	 * Removes the element at the specified position in this list.
	 * Returns the element that was removed from the list.
	 * <p>
	 * Throws an <code>ArrayIndexOutOfBoundsException</code>
	 * if the index is out of range
	 * (<code>index &lt; 0 || index &gt;= size()</code>).
	 *
	 * @param index the index of the element to removed
	 * @return the element previously at the specified position
	 */
	T removeElementAt(size_t index) {
		T old;
		typename Vector::iterator it = delegate.begin();
		it+=index;
		old = *it;
		delegate.erase(it);
		fireIntervalRemoved(index, index);
		return old;
	}
	//-------------------------------------------------------------------------
	/**
	 * Removes all of the elements from this list.  The list will
	 * be empty after this call returns (unless it throws an exception).
	 */
	void clear() {
		size_t index1 = delegate.size() - 1;
		delegate.clear();
		if (index1 >= 0) {
			fireIntervalRemoved(0, index1);
		}
	}
	//-------------------------------------------------------------------------
	/**
	 * Deletes the components at the specified range of indexes.
	 * The removal is inclusive, so specifying a range of (1,5)
	 * removes the component at index 1 and the component at index 5,
	 * as well as all components in between.
	 * <p>
	 * Throws an <code>ArrayIndexOutOfBoundsException</code>
	 * if the index was invalid.
	 * Throws an <code>IllegalArgumentException</code> if
	 * <code>fromIndex &gt; toIndex</code>.
	 *
	 * @param      fromIndex the index of the lower end of the range
	 * @param      toIndex   the index of the upper end of the range
	 * @see        #remove(int)
	 */
	void removeRange(size_t fromIndex, size_t toIndex) {
		typename Vector::iterator from = delegate.begin();
		from+=fromIndex;
		typename Vector::iterator to = delegate.begin();
		to+=toIndex+1;
		delegate.erase(from, to);
		fireIntervalRemoved(fromIndex, toIndex);
	}

}; // DefaultListModel
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTLISTMODEL_H */
