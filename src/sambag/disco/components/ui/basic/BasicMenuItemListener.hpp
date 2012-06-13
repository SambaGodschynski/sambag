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
#include <sambag/disco/components/AButton.hpp>
#include <sambag/com/ICommand.hpp>
#include <sambag/disco/components/events/ActionEvent.hpp>
#include <sambag/disco/components/Window.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicMenuItemListener.
  */
template <class ComponentModell>
struct BasicMenuItemListener {
//=============================================================================
	//-------------------------------------------------------------------------
	static void onMouseEvent(void *src, const events::MouseEvent &ev);
}; // BasicMenuItemListener

///////////////////////////////////////////////////////////////////////////////
template <class ComponentModell>
void BasicMenuItemListener<ComponentModell>::
	onMouseEvent(void *src, const events::MouseEvent &ev)
{
	using namespace events;
	typedef AButton<ComponentModell> AbstractButton;
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(ev.getSource());
	switch(ev.getType()) {
	case MouseEvent::MOUSE_ENTERED:
		b->setButtonRollover(true);
		break;
	case MouseEvent::MOUSE_EXITED:
		b->setButtonRollover(false);
		b->setButtonPressed(false);
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
}}}}} // namespace(s)
#endif /* SAMBAG_BASICMENUITEMLISTENER_H */
