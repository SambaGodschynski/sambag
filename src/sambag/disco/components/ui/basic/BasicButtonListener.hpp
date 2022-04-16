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
#include <sambag/com/ICommand.hpp>
#include <sambag/disco/components/events/ActionEvent.hpp>

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
	typedef AButton<ButtonModell> ButtonClass;
	//-------------------------------------------------------------------------
	typedef typename AButton<ButtonModell>::Ptr ButtonClassPtr;
	//-------------------------------------------------------------------------
	ButtonClassPtr getButton(const events::MouseEvent &ev) const;
	//-------------------------------------------------------------------------
	void mouseEntered(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseExited(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mousePressed(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseReleased(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void onMouseEvent(void *src, const events::MouseEvent &ev);
}; // BasicButtonListener

///////////////////////////////////////////////////////////////////////////////	
//-----------------------------------------------------------------------------
template <class BM>
typename BasicButtonListener<BM>::ButtonClassPtr 
BasicButtonListener<BM>::getButton(const events::MouseEvent &ev) const
{
	return
		boost::dynamic_pointer_cast<ButtonClass>(ev.getSource());
}
//-----------------------------------------------------------------------------
template <class BM>
void BasicButtonListener<BM>::mouseEntered(const events::MouseEvent &ev) {
	
	ButtonClassPtr b = getButton(ev);
	b->setButtonRollover(true);
}
//-----------------------------------------------------------------------------
template <class BM>
void BasicButtonListener<BM>::mouseExited(const events::MouseEvent &ev) {
	
	ButtonClassPtr b = getButton(ev);
	b->setButtonRollover(false);
	b->setButtonPressed(false);
}
//-----------------------------------------------------------------------------
template <class BM>
void BasicButtonListener<BM>::mousePressed(const events::MouseEvent &ev) {
	
	ButtonClassPtr b = getButton(ev);
	if (ev.getButtons() != events::MouseEvent::DISCO_BTN1)
		return;
	b->setButtonPressed(true);
}
//-----------------------------------------------------------------------------
template <class BM>
void BasicButtonListener<BM>::mouseReleased(const events::MouseEvent &ev) {

	ButtonClassPtr b = getButton(ev);
		
	if (ev.getButtons() != events::MouseEvent::DISCO_BTN1)
			return;
	bool oldState = b->isButtonPressed();
	b->setButtonPressed(false);
	if (!oldState)
		return;
	if (!b->contains(ev.getLocation())) {// mouse moved out
		b->setButtonRollover(false);
		return;
	}
	if (!b->isEnabled()) {
		return;
	}
	const sambag::com::ICommand::Function &c =
			b->getButtonFunction();
	if (c) {
		c();
	}
	b->EventSender<events::ActionEvent>::notifyListeners(
			b.get(),
			events::ActionEvent(b)
	);
}
//-----------------------------------------------------------------------------
template <class ButtonModell>
void BasicButtonListener<ButtonModell>::
	onMouseEvent(void *src, const events::MouseEvent &ev)
{
	enum { Filter=
		events::MouseEvent::DISCO_MOUSE_ENTERED |
		events::MouseEvent::DISCO_MOUSE_EXITED  |
		events::MouseEvent::DISCO_MOUSE_PRESSED |
		events::MouseEvent::DISCO_MOUSE_RELEASED 
	};
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
}}}}} // namespace(s)
#endif /* SAMBAG_BASICBUTTONLISTENER_H */
