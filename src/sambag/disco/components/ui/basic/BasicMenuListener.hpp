/*
 * BasicMenuListener.hpp
 *
 *  Created on: Tue Jun  5 15:25:21 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICMENULISTENER_H
#define SAMBAG_BASICMENULISTENER_H

#include <boost/shared_ptr.hpp>
#include "BasicMenuItemListener.hpp"
#include <sambag/disco/components/Menu.hpp>
#include <sambag/disco/components/PopupMenu.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicMenuListener.
  */
template <class ComponentModell>
struct BasicMenuListener : public BasicMenuItemListener<ComponentModell>
{
//=============================================================================
private:
	//-------------------------------------------------------------------------
	static void onMouseEntered(Menu::Ptr item);
	//-------------------------------------------------------------------------
	static void onMouseExited(Menu::Ptr item);
	//-------------------------------------------------------------------------
	static void setupPostTimer(Menu::Ptr item);
public:
	//-------------------------------------------------------------------------
	static void onMouseEvent(void *src, const events::MouseEvent &ev);
}; // BasicMenuListener

///////////////////////////////////////////////////////////////////////////////
template <class ComponentModell>
void BasicMenuListener<ComponentModell>::
	onMouseEvent(void *src, const events::MouseEvent &ev)
{
	using namespace events;
	Menu::Ptr b =
			boost::shared_dynamic_cast<Menu>(ev.getSource());
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
void BasicMenuListener<ComponentModell>::
onMouseEntered(Menu::Ptr item)
{
	item->setButtonRollover(true);
//	// 4188027: drag enter/exit added in JDK 1.1.7A, JDK1.2
//	if ((modifiers & (InputEvent.BUTTON1_MASK | InputEvent.BUTTON2_MASK
//			| InputEvent.BUTTON3_MASK)) != 0) {
//		MenuSelectionManager.defaultManager().processMouseEvent(e);
//	} else {
	IMenuElement::MenuElements path;
	BasicMenuItemListener<ComponentModell>::getPath(path, item);
	if(!(path.size() > 0 &&
			path[path.size()-1] ==
					item->getPopupMenu()))
	{
		MenuSelectionManager::defaultManager().setSelectedPath(path);
		setupPostTimer(item);
	}
}
//-----------------------------------------------------------------------------
struct CallLater : public Timer {
	typedef boost::shared_ptr<CallLater> Ptr;
	Menu::Ptr item;
	static void executeNow(Menu::Ptr item) {
		MenuSelectionManager &defaultManager = MenuSelectionManager::defaultManager();
		IMenuElement::MenuElements path = defaultManager.getSelectedPath();
		if (path.size() > 0 && path[path.size() - 1] == item) {
			//appendPath(path, menu.getPopupMenu());
			path.push_back(item->getPopupMenu());
			defaultManager.setSelectedPath(path);
		}
	}
	/**
	 * @override
	 */
	virtual void timedExpired() {
		executeNow(item);
		Timer::timedExpired();
	}
	static Ptr create(Menu::Ptr item, const Timer::TimeType &time) {
		Ptr neu(new CallLater());
		neu->self = neu;
		neu->item = item;
		neu->setDelay(time);
		return neu;
	}
};
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicMenuListener<ComponentModell>::
setupPostTimer(Menu::Ptr item)
{
	if (item->getDelay() == 0) {
		CallLater::executeNow(item);
		return;
	}
	CallLater::Ptr timer = CallLater::create(item, item->getDelay());
	timer->start();
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicMenuListener<ComponentModell>::
onMouseExited(Menu::Ptr item)
{
	item->setButtonRollover(false);
	item->setButtonPressed(false);
}
}}}}} // namespace(s)
#endif /* SAMBAG_BASICMENULISTENER_H */
