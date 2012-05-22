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
	setEnabled(true);
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isArmed() const {
	return (stateMask & ARMED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isSelected() const {
	return (stateMask & SELECTED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isEnabled() const {
	return (stateMask & ENABLED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isPressed() const {
	return (stateMask & PRESSED) != 0;
}
//-----------------------------------------------------------------------------
bool DefaultButtonModell::isRollover() const {
	return (stateMask & ROLLOVER) != 0;;
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setArmed(bool b) {
	if (isArmed() == b)
		return;
	if (b) {
		stateMask |= ARMED;
	} else {
		stateMask &= ~ARMED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setSelected(bool b) {
	if (isSelected() == b)
		return;
	if (b) {
		stateMask |= SELECTED;
	} else {
		stateMask &= ~SELECTED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setEnabled(bool b) {
	if (isEnabled() == b)
		return;
	if (b) {
		stateMask |= ENABLED;
	} else {
		stateMask &= ~ENABLED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setPressed(bool b) {
	if (isPressed() == b)
		return;
	if (b) {
		stateMask |= PRESSED;
	} else {
		stateMask &= ~PRESSED;
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setRollover(bool b) {
	if (isRollover() == b)
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
void DefaultButtonModell::setGroup(ButtonGroup::Ptr _group) {
	group = _group;
}
//-----------------------------------------------------------------------------
ButtonGroup::Ptr DefaultButtonModell::getGroup() const {
	return group;
}
//-----------------------------------------------------------------------------
void DefaultButtonModell::setActionCommand(sambag::com::ICommand::Ptr cmd )
{
	actionCommand = cmd;
}
//-----------------------------------------------------------------------------
sambag::com::ICommand::Ptr DefaultButtonModell::getActionCommand() const {
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
