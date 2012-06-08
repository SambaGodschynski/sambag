/*
 * AWindow.cpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#include "Window.hpp"
#include "components/RootPane.hpp"
#include "IWindowFactory.hpp"

namespace sambag { namespace disco {
//=============================================================================
//  Class Window
//=============================================================================
//-----------------------------------------------------------------------------
Window::Window(Window::Ptr parent) : parent(parent) {
	rootPane = components::RootPane::create();
	windowImpl = getWindowFactory()->createWindowImpl();
	windowImpl->setRootPane(rootPane);
}
//-----------------------------------------------------------------------------
components::RootPane::Ptr Window::getRootPane() const {
	return rootPane;
}
//-----------------------------------------------------------------------------
void Window::setBounds(const Rectangle &r) {
	windowImpl->setBounds(r);
}
//-----------------------------------------------------------------------------
Rectangle Window::getBounds() const {
	return windowImpl->getBounds();
}
//-----------------------------------------------------------------------------
void Window::open() {
	windowImpl->open();
}
//-----------------------------------------------------------------------------
void Window::close() {
	windowImpl->close();
}
}} // namespace(s)
