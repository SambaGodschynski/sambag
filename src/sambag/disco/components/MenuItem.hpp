/*
 * MenuItem.hpp
 *
 *  Created on: Mon Jun 11 06:33:02 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MENUITEM_H
#define SAMBAG_MENUITEM_H

#include <boost/shared_ptr.hpp>
#include "DefaultButtonModell.hpp"
#include "AButton.hpp"
#include "IMenuElement.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class MenuItem.
  */
class MenuItem :
	public AButton<DefaultButtonModell>,
	public IMenuElement
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<MenuItem> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_DELAY;
private:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> delay;
protected:
	//-------------------------------------------------------------------------
	MenuItem();
public:
	//-------------------------------------------------------------------------
	virtual bool isToplevelMenu() const {
		return false;
	}
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return AButton<Model>::toString();
	}
	//-------------------------------------------------------------------------
	void setDelay(int newDelay);
	//-------------------------------------------------------------------------
	int getDelay() const {
		return delay;
	}
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(MenuItem)
	//-------------------------------------------------------------------------
	/**
	 * @return return the Component used to paint the receiving element.
	 */
	virtual AComponentPtr getComponent() const {
		return getPtr();
	}
	//-------------------------------------------------------------------------
	/**
	 * This method should return an array containing the sub-elements for
	 * the receiving menu element
	 */
	virtual void getSubElements(MenuElements &out) const;
	//-------------------------------------------------------------------------
	/**
	 * Call by the MenuSelectionManager when the MenuElement is added or
	 * remove from the menu selection.
	 */
	virtual void menuSelectionChanged(bool isIncluded);
	//-------------------------------------------------------------------------
	virtual void processMouseEvent(events::MouseEvent event,
			const MenuElements & path, MenuSelectionManager &manager);

}; // MenuItem
}}} // namespace(s)

#endif /* SAMBAG_MENUITEM_H */
