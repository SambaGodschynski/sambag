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
class APopupMenu : public AContainer, public SingleSelectionModell {
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
protected:
	//-------------------------------------------------------------------------
	Point2D location;
	//-------------------------------------------------------------------------
	virtual void onItemClicked(void *src, const events::ActionEvent &ev);
	//-------------------------------------------------------------------------
	APopupMenu();
public:
	//-------------------------------------------------------------------------
	virtual void add(MenuItem::Ptr c);
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
			window->setLocation(p);
		firePropertyChanged(PROPERTY_POPUP_LOCATION, old, p);
	}
}; // PopupMenu
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class SM>
const std::string  APopupMenu<SM>::PROPERTY_POPUP_LOCATION = "popuplocation";
//-----------------------------------------------------------------------------
template <class SM>
APopupMenu<SM>::APopupMenu() {
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
	window = Window::create();
	window->getRootPane()->add(getPtr());
	window->setLocation(location);
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::add(MenuItem::Ptr c) {
	c->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&APopupMenu<SM>::onItemClicked, this, _1, _2)
	);
	AContainer::add(c);
}
//-----------------------------------------------------------------------------
template <class SM>
void APopupMenu<SM>::onItemClicked(void *src, const events::ActionEvent &ev)
{
	//hidePopup();
}
}}} // namespace(s)

#endif /* SAMBAG_APOPUPMENU_H */
