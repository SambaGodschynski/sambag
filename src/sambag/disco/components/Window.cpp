/*
 * AWindow.cpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#include "Window.hpp"
#include "RootPane.hpp"
#include "IWindowFactory.hpp"

namespace sambag { namespace disco { namespace components {
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
//-------------------------------------------------------------------------
void Window::onParentRemove(void *src, const OnCloseEvent &ev) {
	close();
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
	if (parent) {
		parent->addTrackedOnCloseEventListener(
			boost::bind(&Window::onParentRemove, this, _1, _2),
			getPtr()
		);
	}
	rootPane->setParent(getPtr());
	windowImpl->open();
}
//-----------------------------------------------------------------------------
void Window::close() {
	rootPane->setParent(Window::Ptr());
	windowImpl->close();
}
//-----------------------------------------------------------------------------
void Window::pack() {
	// invalidate because:
	// AContainer::(dim == NULL_DIMENSION || !(isPreferredSizeSet() || isValid()))
	rootPane->invalidate();
	setSize(rootPane->getPreferredSize());
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
}}} // namespace(s)
