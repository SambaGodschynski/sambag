/*
 * AWindowImpl.cpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#include "Window.hpp"
#include "RootPane.hpp"
#include "WindowToolkit.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Window
//=============================================================================
//-------------------------------------------------------------------------
void Window::startMainLoop() {
	getWindowToolkit()->startMainLoop();
}
//-----------------------------------------------------------------------------
Window::Window(Window::Ptr parent) : parent(parent) {
	rootPane = components::RootPane::create();
	windowImpl = getWindowToolkit()->createWindowImpl();
	SAMBAG_ASSERT(windowImpl);
	windowImpl->setFramed(false);
	windowImpl->setRootPane(rootPane);
	//add(rootPane); do not call in constructor! -> getPtr() returns NULL
}
//-----------------------------------------------------------------------------
void Window::initWindow() {
	add(rootPane);
	if (parent) {
		parent->addTrackedOnCloseEventListener(
			boost::bind(&Window::onParentRemove, this, _1, _2),
			getPtr()
		);
	}
	windowImpl->EventSender<OnBoundsChanged>::addTrackedEventListener(
			boost::bind(&Window::onBoundsChanged, this, _1, _2),
			getPtr()
	);
}
//-----------------------------------------------------------------------------
void Window::onParentRemove(void *src, const OnCloseEvent &ev) {
	if (windowImpl)
		close();
}
//-----------------------------------------------------------------------------
void Window::onBoundsChanged(void *src, const OnBoundsChanged &ev) {
	//AContainer::setSizeBounds(ev.getNewBounds());
	Dimension n = ev.getNewBounds().getDimension();
	if (n != getSize()) {
		setSize(n);
		validate();
	}
}
//-----------------------------------------------------------------------------
AContainerPtr Window::getRootContainer() const {
	return getPtr();
}
//-----------------------------------------------------------------------------
RootPane::Ptr Window::getRootPane() const {
	return rootPane;
}
//-----------------------------------------------------------------------------
Panel::Ptr Window::getContentPane() const {
	return getRootPane()->getContentPane();
}
//-----------------------------------------------------------------------------
void Window::setWindowBounds(const Rectangle &r) {
	if (!windowImpl->isVisible()) {
		Dimension n = r.getDimension();
		if (n != getSize()) {
			setSize(n);
			validate();
		}
	}
	// container size will be set on WindowImpl resize event
	windowImpl->setBounds(r);
}
//-----------------------------------------------------------------------------
Point2D Window::getLocationOnScreen(const Point2D &p) const {
	if (!isVisible())
		return NULL_POINT2D;
	Point2D tmp = p;
	boost::geometry::add_point(tmp, getWindowLocation());
	return tmp;
}
//-----------------------------------------------------------------------------
Point2D Window::getLocationOnComponent(const Point2D &p) const {
	if (!isVisible())
		return NULL_POINT2D;
	Point2D tmp = p;
	boost::geometry::subtract_point(tmp, getWindowLocation());
	return tmp;
}
//-----------------------------------------------------------------------------
Rectangle Window::getWindowBounds() const {
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
void Window::pack() {
	// invalidate because:
	// AContainer::(dim == NULL_DIMENSION || !(isPreferredSizeSet() || isValid()))
	rootPane->invalidate();
	setWindowSize(rootPane->getPreferredSize());
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
