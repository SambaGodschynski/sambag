/*
 * TestWindow.cpp
 *
 *  Created on: Mon Jun 18 14:48:52 2012
 *      Author: Johannes Unger
 */

#include "TestWindow.hpp"
#include <sambag/disco/components/RootPane.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class TestWindow
//=============================================================================
//-----------------------------------------------------------------------------
void TestWindow::open() {
	isOpen = true;
	root->setSize(bounds.getDimension());
	root->validate();
}
//-----------------------------------------------------------------------------
void TestWindow::close() {
	isOpen = false;
}
//-----------------------------------------------------------------------------
void TestWindow::setBounds(const Rectangle &r) {
	bounds = r;
}
//-----------------------------------------------------------------------------
Rectangle TestWindow::getBounds() const {
	return bounds;
}
//-----------------------------------------------------------------------------
void TestWindow::setRootPane(components::RootPanePtr _root) {
	root = _root;
}
//-----------------------------------------------------------------------------
void TestWindow::setParentWindow(WindowPtr _parent) {
	parent = _parent;
}
//-----------------------------------------------------------------------------
bool TestWindow::isVisible() const {
	return isOpen;
}
//-----------------------------------------------------------------------------
void TestWindow::setTitle(const std::string &_title) {
	title = _title;
}
//-----------------------------------------------------------------------------
std::string TestWindow::getTitle() const {
	return title;
}
}}} // namespace(s)
