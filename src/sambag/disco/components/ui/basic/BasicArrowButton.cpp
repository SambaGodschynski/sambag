/*
 * BasicArrowButton.cpp
 *
 *  Created on: Tue Jul  3 15:29:39 2012
 *      Author: Johannes Unger
 */

#include "BasicArrowButton.hpp"

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
//  Class BasicArrowButton
//=============================================================================
//-----------------------------------------------------------------------------
BasicArrowButton::BasicArrowButton(BasicArrowButton::Direction direction) :
direction(direction)
{
	setName("BasicArrowButton");
}
}}}}} // namespace(s)
