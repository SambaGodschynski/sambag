/*
 * DiscoEvent.cpp
 *
 *  Created on: Mon May 27 13:36:46 2013
 *      Author: Johannes Unger
 */

#include "DiscoEvent.hpp"

namespace sambag { namespace disco { namespace components { namespace events {
//=============================================================================
//  Class DiscoEvent
//=============================================================================
//-----------------------------------------------------------------------------
DiscoEvent::DiscoEvent() : consumed(false) {
}
//-----------------------------------------------------------------------------
void DiscoEvent::consume() const {
	consumed = true;
}
}}}} // namespace(s)
