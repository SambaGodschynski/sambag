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
#include "FlowLayout.hpp"
#include <sambag/disco/components/Window.hpp>
#include "ui/DefaultMenuLayout.hpp"
#include "MenuItem.hpp"


namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class PopupMenu.
  */
template <class SingleSelectionModell>
class APopupMenu :
	public AContainer,
	public IMenuElement,
	public SingleSelectionModell {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<APopupMenu> Ptr;
	//-------------------------------------------------------------------------
	typedef SingleSelectionModell Modell;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_POPUP_LOCATION;
private:
	//-------------------------------------------------------------------------
	void initWindow();
	//-------------------------------------------------------------------------
	WindowPtr window;
	//-------------------------------------------------------------------------
	AComponentPtr parent;
protected:
	//-------------------------------------------------------------------------
	Point2D location;
	//-------------------------------------------------------------------------
	APopupMenu(AComponentPtr parent = AComponentPtr());
public:
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
	virtual void menuSelectionChanged(bool isIncluded){}
	//-------------------------------------------------------------------------
	virtual void processMouseEvent(events::MouseEvent event,
			const MenuElements & path, MenuSelectionManager &manager) {}
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr _parent) {
		parent = _parent;
	}
	//-------------------------------------------------------------------------
	virtual AComponentPtr getParentComponent() const {
		return parent;
	}
	//-------------------------------------------------------------------------
	virtual void showPopup(const Point2D &_where);
	//-------------------------------------------------------------------------
	virtual void hidePopup();
	//-------------------------------------------------------------------------
	virtual bool isPopupVisible() const {
		if (!window)
			return false;
		return window->isVisible();
	}
	//-------------------------------------------------------------------------
	virtual void setPopupLocation(const Point2D &p) {
		Point2D old = location;
		if (old == p)
			return;
		SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
			location = p;
		SAMBAG_END_SYNCHRONIZED
		if (window)
			window->setWindowLocation(p);
		firePropertyChanged(PROPERTY_POPUP_LOCATION, old, p);
	}
}; // PopupMenu
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class SM>
const std::string  APopupMenu<SM>::PROPERTY_POPUP_LOCATION = "popuplocation";
//-----------------------------------------------------------------------------
template <class SM>
APopupMenu<SM>::APopupMenu(AComponentPtr parent) : parent(parent) {
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::showPopup(const Point2D &_where) {
	setPopupLocation(_where);
	if (!window)
		initWindow();
	window->pack();
	window->open();
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::hidePopup() {
	window->close();
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::initWindow() {
	setLayout(
		ui::DefaultMenuLayout::create(getPtr(), ui::DefaultMenuLayout::Y_AXIS)
	);
	Window::Ptr parentW;
	if (parent) {
		AContainerPtr pc = parent->getRootContainer();
		parentW = boost::shared_dynamic_cast<Window>(pc);
	}
	window = Window::create(parentW);
	window->getRootPane()->add(getPtr());
	window->setWindowLocation(location);
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::getSubElements(MenuElements &out) const {
	for (size_t i=0; i<getComponentCount(); ++i) {
		AComponentPtr comp = AContainer::getComponent(i);
		IMenuElement::Ptr el = boost::shared_dynamic_cast<IMenuElement>(comp);
		if (!el)
			continue;
		out.push_back(el);
	}
}

}}} // namespace(s)

#endif /* SAMBAG_APOPUPMENU_H */
