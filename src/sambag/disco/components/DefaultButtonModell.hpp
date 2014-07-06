/*
 * DefaultButtonModell.hpp
 *
 *  Created on: Sun Jun  3 17:47:22 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULT_BUTTONMODELL_H
#define SAMBAG_DEFAULT_BUTTONMODELL_H

#include "ButtonGroup.hpp"
#include <sambag/com/ICommand.hpp>
#include <sambag/com/events/ChangedEvent.hpp>
#include <sambag/com/events/Events.hpp>
#include "events/ActionEvent.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace sambag { namespace disco { namespace components {
namespace ev = sambag::com::events;
class DefaultButtonModell;
typedef ev::ChangedEvent<DefaultButtonModell> DefaultButtonModellChangedEvent;
//=============================================================================
/** 
  * @class DefaultButtonModell Policy.
  */
class DefaultButtonModell :
	public ev::EventSender<DefaultButtonModellChangedEvent>,
	public ev::EventSender<events::ActionEvent>
{
//=============================================================================
public:
	typedef ev::ChangedEvent<DefaultButtonModell> StateChangedEvent;
    typedef boost::shared_ptr<DefaultButtonModell> Ptr;
    typedef boost::weak_ptr<DefaultButtonModell> WPtr;
protected:
	//-------------------------------------------------------------------------
	/** The bitmask used to store the state of the button. */
	int stateMask;
	//-------------------------------------------------------------------------
	/** The action command string fired by the button. */
	sambag::com::ICommand::Function action;
	//-------------------------------------------------------------------------
	/** The button group that the button belongs to. */
	ButtonGroup::Ptr group;
	//-------------------------------------------------------------------------
	/** The button's mnemonic. */
	int mnemonic;
	//-------------------------------------------------------------------------
	std::string text;
	//-------------------------------------------------------------------------
	void fireStateChanged();
public:
	//-------------------------------------------------------------------------
	virtual ~DefaultButtonModell() {}
	//-------------------------------------------------------------------------
	static const int ARMED = 1;
	//-------------------------------------------------------------------------
	static const int SELECTED = 1 << 1;
	//-------------------------------------------------------------------------
	static const int PRESSED = 1 << 2;
	//-------------------------------------------------------------------------
	static const int ENABLED = 1 << 3;
	//-------------------------------------------------------------------------
	static const int ROLLOVER = 1 << 4;
	//-------------------------------------------------------------------------
	DefaultButtonModell();
	//-------------------------------------------------------------------------
	/**
	 * Indicates partial commitment towards triggering the
	 * button.
	 *
	 * @return <code>true</code> if the button is armed,
	 *         and ready to be triggered
	 * @see #setButtonArmed
	 */
	bool isButtonArmed() const;
	//-------------------------------------------------------------------------
	/**
	 * Indicates if the button has been selected. Only needed for
	 * certain types of buttons - such as radio buttons and check boxes.
	 *
	 * @return <code>true</code> if the button is selected
	 */
	bool isButtonSelected() const;
	//-------------------------------------------------------------------------
	/**
	 * Indicates if the button can be selected or triggered by
	 * an input device, such as a mouse pointer.
	 *
	 * @return <code>true</code> if the button is enabled
	 */
	bool isButtonEnabled() const;
	//-------------------------------------------------------------------------
	/**
	 * Indicates if the button is pressed.
	 *
	 * @return <code>true</code> if the button is pressed
	 */
	bool isButtonPressed() const;
	//-------------------------------------------------------------------------
	/**
	 * Indicates that the mouse is over the button.
	 *
	 * @return <code>true</code> if the mouse is over the button
	 */
	bool isButtonRollover() const;
	//-------------------------------------------------------------------------
	/**
	 * Marks the button as armed or unarmed.
	 *
	 * @param b whether or not the button should be armed
	 */
	void setButtonArmed(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Selects or deselects the button.
	 *
	 * @param b <code>true</code> selects the button,
	 *          <code>false</code> deselects the button
	 */
	void setButtonSelected(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Enables or disables the button.
	 *
	 * @param b whether or not the button should be enabled
	 * @see #isButtonEnabled
	 */
	void setButtonEnabled(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Sets the button to pressed or unpressed.
	 *
	 * @param b whether or not the button should be pressed
	 * @see #isButtonPressed
	 */
	void setButtonPressed(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Sets or clears the button's rollover state
	 *
	 * @param b whether or not the button is in the rollover state
	 * @see #isButtonRollover
	 */
	void setButtonRollover(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Sets the keyboard mnemonic (shortcut key or
	 * accelerator key) for the button.
	 *
	 * @param key an int specifying the accelerator key
	 */
	void setMnemonic(int key);
	//-------------------------------------------------------------------------
	/**
	 * Gets the keyboard mnemonic for the button.
	 *
	 * @return an int specifying the accelerator key
	 * @see #setMnemonic
	 */
	int getMnemonic() const;
	//-------------------------------------------------------------------------
	/**
	 * Identifies the group the button belongs to --
	 * needed for radio buttons, which are mutually
	 * exclusive within their group.
	 *
	 * @param group the <code>ButtonGroup</code> the button belongs to
	 */
	void setButtonGroup(ButtonGroup::Ptr group);
	//-------------------------------------------------------------------------
	ButtonGroup::Ptr getButtonGroup() const;
	//-------------------------------------------------------------------------
	void setButtonFunction(const sambag::com::ICommand::Function &action);
	//-------------------------------------------------------------------------
	const sambag::com::ICommand::Function & getButtonFunction() const;
}; // IButtonModell
}}} // namespace(s)

#endif
