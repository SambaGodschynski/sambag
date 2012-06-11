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
void Window::startMainLoop() {
	__startWindowImplMainLoop_();
}
//-----------------------------------------------------------------------------
Window::Window(Window::Ptr parent) : parent(parent) {
	rootPane = components::RootPane::create();
	windowImpl = getWindowFactory()->createWindowImpl();
	windowImpl->setFramed(false);
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
//-----------------------------------------------------------------------------
Window::OnCloseEventSender::Connection
Window::addOnCloseEventListener(
		const Window::OnCloseEventSender::EventFunction &f
)
{
	return windowImpl->EventSender<OnCloseEvent>::addEventListener(f);
}
//-----------------------------------------------------------------------------
Window::OnCloseEventSender::Connection
Window::addTrackedOnCloseEventListener(
		const Window::OnCloseEventSender::EventFunction &f,
		Window::AnyWPtr wPtr
)
{
	return
		windowImpl->EventSender<OnCloseEvent>::addTrackedEventListener(f, wPtr);
}
}} // namespace(s)
