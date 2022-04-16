/*
 * TestWindowImpl.cpp
 *
 *  Created on: Mon Jun 18 14:48:52 2012
 *      Author: Johannes Unger
 */

#include "TestWindowImpl.hpp"
#include <sambag/disco/components/RootPane.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class TestWindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
void TestWindowImpl::open(AWindowImplPtr parent) {
	isOpen = true;
	root->setSize(bounds.getDimension());
	root->validate();
}
//-----------------------------------------------------------------------------
void TestWindowImpl::invalidateWindow(const Rectangle &area) {
}
//-----------------------------------------------------------------------------
void TestWindowImpl::close() {
	isOpen = false;
}
//-----------------------------------------------------------------------------
void TestWindowImpl::setBounds(const Rectangle &r) {
	bounds = r;
}
//-----------------------------------------------------------------------------
Rectangle TestWindowImpl::getBounds() const {
	return bounds;
}
//-----------------------------------------------------------------------------
void TestWindowImpl::setRootPane(components::RootPanePtr _root) {
	root = _root;
}
//-----------------------------------------------------------------------------
void TestWindowImpl::setParentWindow(WindowPtr _parent) {
	parent = _parent;
}
//-----------------------------------------------------------------------------
bool TestWindowImpl::isVisible() const {
	return isOpen;
}
//-----------------------------------------------------------------------------
void TestWindowImpl::setTitle(const std::string &_title) {
	title = _title;
}
//-----------------------------------------------------------------------------
std::string TestWindowImpl::getTitle() const {
	return title;
}
}}} // namespace(s)
