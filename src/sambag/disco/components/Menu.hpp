/*
 * Menu.hpp
 *
 *  Created on: Mon Jun 11 06:32:54 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MENU_H
#define SAMBAG_MENU_H

#include <boost/shared_ptr.hpp>
#include "MenuItem.hpp"
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Menu.
  */
class Menu : public MenuItem {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Menu> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	PopupMenuPtr popupMenu;
	//-------------------------------------------------------------------------
	Menu();
	//-------------------------------------------------------------------------
	virtual void constructorAlt();
private:
public:
	//-------------------------------------------------------------------------
	PopupMenuPtr getPopupMenu() const {
		return popupMenu;
	}
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Menu)
	//-------------------------------------------------------------------------
	virtual void add(AComponent::Ptr comp, size_t index = -1);
	//-------------------------------------------------------------------------
	void setPopupMenuVisible(bool b);
	//-------------------------------------------------------------------------
	bool isPopupMenuVisible() const;
}; // Menu
}}} // namespace(s)

#endif /* SAMBAG_MENU_H */
