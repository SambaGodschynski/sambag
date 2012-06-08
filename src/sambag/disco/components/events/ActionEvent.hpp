/*
 * ActionEvent.hpp
 *
 *  Created on: Fri Jun  8 13:41:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ACTIONEVENT_H
#define SAMBAG_ACTIONEVENT_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/Forward.hpp>
namespace sambag { namespace disco { namespace components { namespace events {

//=============================================================================
/** 
  * @class ActionEvent.
  */
class ActionEvent {
//=============================================================================
	AComponentPtr src;
public:
	ActionEvent(AComponentPtr src) : src(src) {}
	AComponentPtr getSource() const { return src; }

}; // ActionEvent
}}}} // namespace(s)

#endif /* SAMBAG_ACTIONEVENT_H */
