/*
 * PopupMenu.hpp
 *
 *  Created on: Mon Jun 11 11:19:42 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_POPUPMENU_H
#define SAMBAG_POPUPMENU_H

#include <boost/shared_ptr.hpp>
#include "APopupMenu.hpp"
#include "DefaultSingleSelectionModell.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class PopupMenu.
  */
class PopupMenu : public APopupMenu<DefaultSingleSelectionModell> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<PopupMenu> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
private:
protected:
	//-------------------------------------------------------------------------
	PopupMenu();
public:
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(PopupMenu)
}; // PopupMenu
}}} // namespace(s)

#endif /* SAMBAG_POPUPMENU_H */
