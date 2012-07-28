/*
 * ContainerEvent.hpp
 *
 *  Created on: Tue May 22 15:06:09 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CONTAINEREVENT_H
#define SAMBAG_CONTAINEREVENT_H

#include <sambag/disco/components/Forward.hpp>

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
/** 
  * @class ContainerEvent.
  */
class ContainerEvent {
//=============================================================================
public:
	enum Type {
		//This event indicates that a component was added to the container.
		COMPONENT_ADDED,
		//This event indicates that a component was removed from the container.
		COMPONENT_REMOVED
	};
private:
	AContainerPtr origin;
	Type type;
	AComponentPtr child;
public:
	ContainerEvent(AContainerPtr origin, Type type, AComponentPtr child) :
		origin(origin),
		type(type),
		child(child) {}
	AContainerPtr getOrigin() const  {
		return origin;
	}
	AComponentPtr getChild() const {
		return child;
	}
	Type getType() const {
		return type;
	}

}; // ContainerEvent
}}}} // namespace(s)

#endif /* SAMBAG_CONTAINEREVENT_H */
