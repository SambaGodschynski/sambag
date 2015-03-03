/*
 * Menu.hpp
 *
 *  Created on: Mon Jun 11 06:32:54 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MENU_H
#define SAMBAG_MENU_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
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
	typedef boost::weak_ptr<Menu> WPtr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	PopupMenuPtr popupMenu;
	//-------------------------------------------------------------------------
	Menu();
	//-------------------------------------------------------------------------
	virtual void postConstructor();
private:
	//-------------------------------------------------------------------------
	/**
	 * @return popup location
	 */
	Point2D computePopupLocation() const;
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
	virtual void remove(AComponent::Ptr comp);
    //-------------------------------------------------------------------------
	virtual void removeAll();
	//-------------------------------------------------------------------------
	void setPopupMenuVisible(bool b);
	//-------------------------------------------------------------------------
	bool isPopupMenuVisible() const;
}; // Menu
}}} // namespace(s)

#endif /* SAMBAG_MENU_H */
