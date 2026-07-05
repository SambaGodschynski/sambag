/*
 * IButtonUI.hpp
 *
 *  Created on: Tue Jun  5 06:37:34 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IBUTTONUI_H
#define SAMBAG_IBUTTONUI_H

#include <memory>
#include "AComponentUI.hpp"
namespace sambag { namespace disco { namespace components { namespace ui {

//=============================================================================
/** 
  * @class IButtonUI.
  */
class IButtonUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::shared_ptr<IButtonUI> Ptr;
}; // IButtonUI
}}}} // namespace(s)

#endif /* SAMBAG_IBUTTONUI_H */
