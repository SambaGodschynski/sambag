/*
 * DefaultButtonModell.cpp
 *
 *  Created on: Jun 4, 2012
 *      Author: samba
 */
#include "DefaultButtonModell.hpp"
#include <sambag/com/Common.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
// DefaultButtonModell
//=============================================================================
//-----------------------------------------------------------------------------
DefaultButtonModell::DefaultButtonModell() :
		stateMask(0),
		mnemonic(0)
{
	setButtonEnabled(true);
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isButtonArmed() const {
	return (stateMask & ARMED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isButtonSelected() const {
	return (stateMask & SELECTED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isButtonEnabled() const {
	return (stateMask & ENABLED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isButtonPressed() const {
	return (stateMask & PRESSED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isButtonRollover() const {
	return (stateMask & ROLLOVER) != 0;;
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonArmed(bool b) {
	if (isButtonArmed() == b)
		return;
	if (b) {
		stateMask |= ARMED;
	} else {
		stateMask &= ~ARMED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonSelected(bool b) {
	if (isButtonSelected() == b)
		return;
	if (b) {
		stateMask |= SELECTED;
	} else {
		stateMask &= ~SELECTED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonEnabled(bool b) {
	if (isButtonEnabled() == b)
		return;
	if (b) {
		stateMask |= ENABLED;
	} else {
		stateMask &= ~ENABLED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonPressed(bool b) {
	if (isButtonPressed() == b)
		return;
	if (b) {
		stateMask |= PRESSED;
	} else {
		stateMask &= ~PRESSED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonRollover(bool b) {
	if (isButtonRollover() == b)
		return;
	if (b) {
		stateMask |= ROLLOVER;
	} else {
		stateMask &= ~ROLLOVER;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setMnemonic(int key) {
	mnemonic = key;
	fireStateChanged();
}
//-----------------------------------------------------------------------------
int DefaultButtonModell::getMnemonic() const {
	return mnemonic;
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonGroup(ButtonGroup::Ptr _group) {
	group = _group;
}
//-----------------------------------------------------------------------------
ButtonGroup::Ptr DefaultButtonModell::getButtonGroup() const {
	return group;
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setButtonCommand(sambag::com::ICommand::Ptr cmd )
{
	actionCommand = cmd;
}
//-----------------------------------------------------------------------------
sambag::com::ICommand::Ptr DefaultButtonModell::getButtonCommand() const {
	return actionCommand;
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::fireStateChanged() {
	EventSender<DefaultButtonModellChangedEvent>::notifyListeners(
			this,
			DefaultButtonModellChangedEvent(*this)
	);
}
}}}
