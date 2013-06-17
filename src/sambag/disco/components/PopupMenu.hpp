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
#include "Forward.hpp"

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
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
private:
protected:
	//-------------------------------------------------------------------------
	PopupMenu(AComponentPtr parent = AComponentPtr());
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::dynamic_pointer_cast<PopupMenu>(AComponent::getPtr());
	}
	//-------------------------------------------------------------------------
	static Ptr create(AComponentPtr parent = AComponentPtr()) {
		Ptr neu(new PopupMenu(parent));
		neu->self = neu;
		return neu;
	}
}; // PopupMenu
}}} // namespace(s)

#endif /* SAMBAG_POPUPMENU_H */
