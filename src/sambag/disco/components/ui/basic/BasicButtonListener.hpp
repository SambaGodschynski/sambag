/*
 * BasicButtonListener.hpp
 *
 *  Created on: Tue Jun  5 15:25:21 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICBUTTONLISTENER_H
#define SAMBAG_BASICBUTTONLISTENER_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/AButton.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicButtonListener.
  */
template <class ButtonModell>
struct BasicButtonListener {
//=============================================================================
	//-------------------------------------------------------------------------
	static void onMouseEvent(void *src, const events::MouseEvent &ev);
}; // BasicButtonListener

//=============================================================================
template <class ButtonModell>
void BasicButtonListener<ButtonModell>::
	onMouseEvent(void *src, const events::MouseEvent &ev)
{
	using namespace events;
	typedef AButton<ButtonModell> AbstractButton;
	typename AbstractButton::Ptr b =
			boost::shared_dynamic_cast<AbstractButton>(ev.getSource());
	switch(ev.getType()) {
	case MouseEvent::MOUSE_ENTERED:
		b->setRollover(true); break;
	case MouseEvent::MOUSE_EXITED:
		b->setRollover(false); break;
	case MouseEvent::MOUSE_PRESSED:
		b->setPressed(true); break;
	case MouseEvent::MOUSE_RELEASED:
		b->setPressed(false); break;
	default:
		break;
	}
}
}}}}} // namespace(s)
#endif /* SAMBAG_BASICBUTTONLISTENER_H */