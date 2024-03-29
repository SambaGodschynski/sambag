/*
 * PopupMenu.hpp
 *
 *  Created on: Mon Jun 11 10:55:36 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_APOPUPMENU_H
#define SAMBAG_APOPUPMENU_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "AContainer.hpp"
#include "FlowLayout.hpp"
#include <sambag/disco/components/Window.hpp>
#include "ui/DefaultMenuLayout.hpp"
#include "MenuItem.hpp"
#include "Menu.hpp"


namespace sambag { namespace disco { namespace components {
namespace popupEvents {
    struct BeforeShowingEvent {};
    struct BeforeHidingEvent {};
};
//=============================================================================
/** 
  * @class PopupMenu.
  */
template <class SingleSelectionModell>
class APopupMenu :
	public AContainer,
	public IMenuElement,
	public SingleSelectionModell,
    public com::events::EventSender<OnOpenEvent>,
    public com::events::EventSender<OnCloseEvent>,
    public com::events::EventSender<popupEvents::BeforeShowingEvent>,
    public com::events::EventSender<popupEvents::BeforeHidingEvent>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef SingleSelectionModell Model;
	//-------------------------------------------------------------------------
    typedef APopupMenu<Model> Class;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Class> WPtr;
    //-------------------------------------------------------------------------
	static const std::string PROPERTY_POPUP_LOCATION;
    //-------------------------------------------------------------------------
    typedef popupEvents::BeforeShowingEvent BeforeShowingEvent;
    //-------------------------------------------------------------------------
    typedef popupEvents::BeforeHidingEvent BeforeHidingEvent;   
private:
	//-------------------------------------------------------------------------
	AComponentWPtr _invoker;
	//-------------------------------------------------------------------------
	WindowPtr window;
    //-------------------------------------------------------------------------
    template <class Event>
    void dispatch(const Event &ev) {
        com::events::EventSender<Event>::notifyListeners(this, ev);
    }
    //-------------------------------------------------------------------------
	void initWindow();
protected:
	//-------------------------------------------------------------------------
	/**
     * @brief creates the menu window
     * @param the parent window
     */
    virtual WindowPtr createWindow(WindowPtr parent);
	//-------------------------------------------------------------------------
	Point2D location;
	//-------------------------------------------------------------------------
	APopupMenu(AComponentPtr invoker = AComponentPtr());
public:
    //-------------------------------------------------------------------------
    WindowPtr getPopupWindow() const {
        return window;
    }
	//-------------------------------------------------------------------------
	void setInvoker(AComponentPtr _invoker);
	//-------------------------------------------------------------------------
	AComponentPtr getInvoker() const;
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return AContainer::toString();
	}
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
			const MenuElements & path, MenuSelectionManager &manager) {}
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
APopupMenu<SM>::APopupMenu(AComponentPtr invoker) : _invoker(invoker) {
	setName("PopupMenu");
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::showPopup(const Point2D &_where) {

    com::events::EventSender<BeforeShowingEvent>::notifyListeners(this,
        BeforeShowingEvent());

	setPopupLocation(_where);
	if (!window)
		initWindow();
	window->pack();
	window->open();
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::hidePopup() {

    com::events::EventSender<BeforeHidingEvent>::notifyListeners(this,
        BeforeHidingEvent());

	if (window) {
		window->close();
	}
}
//-----------------------------------------------------------------------------
template <class SM>
WindowPtr APopupMenu<SM>::createWindow(WindowPtr parent) {
    return Window::create(parent);
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::initWindow() {
	setLayout(
		ui::DefaultMenuLayout::create(getPtr(), ui::DefaultMenuLayout::Y_AXIS)
	);
	Window::Ptr parentW;
	AComponentPtr invoker = getInvoker();
	if (invoker) {
		parentW = invoker->getTopLevelAncestor();
	}
	window = createWindow(parentW);
	window->getContentPane()->add(AsWeakPtr<AComponent>(getPtr()));
	window->setWindowLocation(location);
    window->addTrackedOnOpenEventListener(
        boost::bind(&Class::template dispatch<OnOpenEvent>, this, _2),
        getPtr()
    );
    window->addTrackedOnCloseEventListener(
        boost::bind(&Class::template dispatch<OnCloseEvent>, this, _2),
        getPtr()
    );
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::getSubElements(MenuElements &out) const {
	for (size_t i=0; i<getComponentCount(); ++i) {
		AComponentPtr comp = AContainer::getComponent(i);
		IMenuElement::Ptr el = boost::dynamic_pointer_cast<IMenuElement>(comp);
		if (!el)
			continue;
		out.push_back(el);
	}
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::setInvoker(AComponentPtr invoker) {
	_invoker = invoker;
}
//-----------------------------------------------------------------------------
template <class SM>
AComponentPtr APopupMenu<SM>::getInvoker() const {
	return _invoker.lock();
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::menuSelectionChanged(bool isIncluded) {
	Menu::Ptr m = boost::dynamic_pointer_cast<Menu>(getInvoker());
	if (m)
		m->setPopupMenuVisible(isIncluded);
	else
		hidePopup();
}
}}} // namespace(s)

#endif /* SAMBAG_APOPUPMENU_H */
