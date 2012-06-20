/*
 * BasicMenuItemListener.hpp
 *
 *  Created on: Tue Jun  5 15:25:21 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICMENUITEMLISTENER_H
#define SAMBAG_BASICMENUITEMLISTENER_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/MenuItem.hpp>
#include <sambag/com/ICommand.hpp>
#include <sambag/disco/components/events/ActionEvent.hpp>
#include <sambag/disco/components/Window.hpp>
#include <sambag/disco/components/IMenuElement.hpp>
#include <sambag/disco/components/MenuSelectionManager.hpp>
#include <algorithm>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicMenuItemListener.
  */
template <class ComponentModell>
struct BasicMenuItemListener {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	static void onMouseEntered(MenuItem::Ptr item);
	//-------------------------------------------------------------------------
	static void onMouseExited(MenuItem::Ptr item);
public:
	//-------------------------------------------------------------------------
	static void onMouseEvent(void *src, const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	static void getPath(IMenuElement::MenuElements &out, MenuItem::Ptr c);
}; // BasicMenuItemListener

///////////////////////////////////////////////////////////////////////////////
template <class ComponentModell>
void BasicMenuItemListener<ComponentModell>::
	onMouseEvent(void *src, const events::MouseEvent &ev)
{
	using namespace events;
	MenuItem::Ptr b =
			boost::shared_dynamic_cast<MenuItem>(ev.getSource());
	SAMBAG_ASSERT(b);
	switch(ev.getType()) {
	case MouseEvent::MOUSE_ENTERED:
		onMouseEntered(b);
		break;
	case MouseEvent::MOUSE_EXITED:
		onMouseExited(b);
		break;
	case MouseEvent::MOUSE_PRESSED:
		if (ev.getButtons() != MouseEvent::BUTTON1)
			break;
		b->setButtonPressed(true);
		break;
	case MouseEvent::MOUSE_RELEASED: {
		if (ev.getButtons() != MouseEvent::BUTTON1)
			break;
		bool oldState = b->isButtonPressed();
		b->setButtonPressed(false);
		if (!oldState)
			break;
		sambag::com::ICommand::Ptr c =
				b->getButtonCommand();
		if (c)
			c->execute();
		b->EventSender<events::ActionEvent>::notifyListeners(
				b.get(),
				events::ActionEvent(b)
		);
		Window::Ptr w = boost::shared_dynamic_cast<Window>(b->getRootContainer());
		if (w)
			w->close();
		break;
	}
	default:
		break;
	}
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicMenuItemListener<ComponentModell>::
onMouseEntered(MenuItem::Ptr item)
{
	item->setButtonRollover(true);
//	// 4188027: drag enter/exit added in JDK 1.1.7A, JDK1.2
//	if ((modifiers & (InputEvent.BUTTON1_MASK | InputEvent.BUTTON2_MASK
//			| InputEvent.BUTTON3_MASK)) != 0) {
//		MenuSelectionManager.defaultManager().processMouseEvent(e);
//	} else {
	IMenuElement::MenuElements path;
	getPath(path, item);
	MenuSelectionManager::defaultManager().setSelectedPath(path);
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicMenuItemListener<ComponentModell>::
onMouseExited(MenuItem::Ptr item)
{
	item->setButtonRollover(false);
	item->setButtonPressed(false);
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicMenuItemListener<ComponentModell>::
getPath(IMenuElement::MenuElements &out, MenuItem::Ptr menuItem)
{
	typedef IMenuElement::MenuElements MenuElements;
	MenuSelectionManager &m = MenuSelectionManager::defaultManager();
	MenuElements oldPath = m.getSelectedPath();
	int i = oldPath.size();
	if (i == 0)
		return;
	AComponentPtr parent = menuItem->getParent();
	if (oldPath[i - 1]->getComponent() == parent) {
		// The parent popup menu is the last so far
		out.resize(i + 1);
		//System.arraycopy(oldPath, 0, newPath, 0, i);
		std::copy(oldPath.begin(), oldPath.begin() + i, out.begin());
		out[i] = menuItem;
	} else {
		// A sibling menuitem is the current selection
		//
		//  This probably needs to handle 'exit submenu into
		// a menu item.  Search backwards along the current
		// selection until you find the parent popup menu,
		// then copy up to that and add yourself...
		int j;
		for (j = oldPath.size() - 1; j >= 0; --j) {
			if (oldPath[j]->getComponent() == parent)
				break;
		}
		out.resize(j + 2);
		//System.arraycopy(oldPath, 0, newPath, 0, j + 1);
		std::copy(oldPath.begin(), oldPath.begin() + (j + 1), out.begin());
		out[j + 1] = menuItem;
	}
 }
}}}}} // namespace(s)
#endif /* SAMBAG_BASICMENUITEMLISTENER_H */
