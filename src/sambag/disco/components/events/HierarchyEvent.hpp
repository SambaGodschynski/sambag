/*
 * HierarchyEvent.hpp
 *
 *  Created on: Mon May 21 15:15:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_HIERARCHYEVENT_H
#define SAMBAG_HIERARCHYEVENT_H

#include <sambag/disco/components/Forward.hpp>

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
/** 
  * @class HierachyEvent.
  */
class HierarchyEvent {
//=============================================================================
public:
	enum Type {
		// The event type indicating an ancestor-Container was moved.
		ANCESTOR_MOVED		= 1,
		// The event type indicating an ancestor-Container was resized.
		ANCESTOR_RESIZED	= 1<<2,
		// Indicates that the HIERARCHY_CHANGED event was generated due to a
		// change in the displayability of the hierarchy.
		DISPLAYABILITY_CHANGED = 1<<3,
		// The event type indicating that modification was made to the entire
		// hierarchy tree.
		HIERARCHY_CHANGED = 1<<4,
		// Marks the first integer type for the range of hierarchy event types.
		HIERARCHY_FIRST = 1<<5,
		// Marks the last integer type for the range of ancestor event types.
		HIERARCHY_LAST = 1<<6,
		// Indicates that the HIERARCHY_CHANGED event was generated by a
		// reparenting operation.
		PARENT_CHANGED = 1<<7,
		// Indicates that the HIERARCHY_CHANGED event was generated due
		// to a change in the showing state of the hierarchy.
		SHOWING_CHANGED = 1<<8
	};
private:
	AComponentPtr source;
	Type type;
	AComponentPtr changed;
	AContainerPtr changedParent;
	size_t changeFlags;
public:
	HierarchyEvent( AComponentPtr source,
			Type type,
			AComponentPtr changed,
			AContainerPtr changedParent,
			size_t changeFlags = 0)
	: source(source),
	  type(type),
	  changed(changed),
	  changedParent(changedParent),
	  changeFlags(changeFlags)
	{
	}
	AContainerPtr getChangedParent() const  {
		return changedParent;
	}
	AComponentPtr getChanged() const {
		return changed;
	}
	AComponentPtr getSource() const {
		return source;
	}
	size_t getChangedFlags() const {
		return changeFlags;
	}
}; // HierarchyEvent
}}}} // namespace(s)

#endif /* SAMBAG_HIERACHYEVENT_H */
