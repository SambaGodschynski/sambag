/*
 * MenuSelectionManager.hpp
 *
 *  Created on: Thu Jun 14 13:23:23 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MENUSELECTIONMANAGER_H
#define SAMBAG_MENUSELECTIONMANAGER_H

#include <boost/shared_ptr.hpp>
#include "MenuItem.hpp"
#include "IMenuElement.hpp"
#include "loki/Singleton.h"
#include <sambag/com/events/ChangedEvent.hpp>
#include <ostream>

namespace sambag { namespace disco { namespace components {
namespace sce = sambag::com::events;
class MenuSelectionManager;
typedef sce::ChangedEvent<MenuSelectionManager> MenuSelectionManagerChanged;
//=============================================================================
/** 
  * @class MenuSelectionManager.
  */
class MenuSelectionManager :
	public sce::EventSender<MenuSelectionManagerChanged> {
//=============================================================================
friend struct Loki::CreateUsingNew<MenuSelectionManager>;
public:
	//-------------------------------------------------------------------------
	typedef IMenuElement::MenuElements MenuElements;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<MenuSelectionManager> Ptr;
protected:
private:
	//-------------------------------------------------------------------------
	MenuElements selection;
	//-------------------------------------------------------------------------
	MenuSelectionManager() {}
	//-------------------------------------------------------------------------
	MenuSelectionManager(const MenuSelectionManager&) {}
	//-------------------------------------------------------------------------
	bool isComponentPartOfCurrentMenu
	(IMenuElement::Ptr root, AComponentPtr c) const;
public:
	//-------------------------------------------------------------------------
	// Adds a ChangeListener to the button.
	//void	addChangeListener(ChangeListener l)
	//-------------------------------------------------------------------------
	// Tell the menu selection to close and unselect all the menu components.
	void clearSelectedPath();
	//-------------------------------------------------------------------------
	// Returns the component in the currently selected path which contains sourcePoint.
	AComponentPtr componentForPoint(AComponentPtr source,
			const Point2D &sourcePoint) const;
	//-------------------------------------------------------------------------
	// Returns the default menu selection manager.
	static MenuSelectionManager & defaultManager();
protected:
	//-------------------------------------------------------------------------
	//Notifies all listeners that have registered interest for notification on this event type.
	void fireStateChanged();
public:
	//-------------------------------------------------------------------------
	//  Returns the path to the currently selected menu item
	const MenuElements & getSelectedPath() const {
		return selection;
	}
	//-------------------------------------------------------------------------
	// Return true if c is part of the currently used menu
	bool isComponentPartOfCurrentMenu(AComponentPtr c) const;
	//-------------------------------------------------------------------------
	// Changes the selection in the menu hierarchy.
	void setSelectedPath(const MenuElements &path);
	//-------------------------------------------------------------------------
	void processMouseEvent(const events::MouseEvent &ev);
}; // MenuSelectionManager
///////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &os,
		const IMenuElement::MenuElements &path);
}}} // namespace(s)

#endif /* SAMBAG_MENUSELECTIONMANAGER_H */
