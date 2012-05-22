/*
 * ChangedEvent.hpp
 *
 *  Created on: Mon Jun  4 12:04:39 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CHANGEDEVENT_H
#define SAMBAG_CHANGEDEVENT_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace com { namespace events {
//=============================================================================
/** 
  * @class ChangedEvent.
  */
template <typename T>
class ChangedEvent {
//=============================================================================
protected:
	//-------------------------------------------------------------------------
	const T & src;
public:
	//-------------------------------------------------------------------------
	ChangedEvent(const T &src) : src(src) {}
	//-------------------------------------------------------------------------
	const T & getSrc() const { return src; }
};
}}} // namespace(s)

#endif /* SAMBAG_CHANGEDEVENT_H */
