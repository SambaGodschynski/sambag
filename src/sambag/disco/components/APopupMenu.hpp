/*
 * PopupMenu.hpp
 *
 *  Created on: Mon Jun 11 10:55:36 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_APOPUPMENU_H
#define SAMBAG_APOPUPMENU_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class PopupMenu.
  */
template <class SingleSelectionModell>
class APopupMenu : public AContainer, public SingleSelectionModell {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<APopupMenu> Ptr;
	//-------------------------------------------------------------------------
	typedef SingleSelectionModell Modell;
private:
protected:
	//-------------------------------------------------------------------------
	APopupMenu() {}
public:
}; // PopupMenu
}}} // namespace(s)

#endif /* SAMBAG_APOPUPMENU_H */
