/*
 * AWindowImpl.cpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#include "Window.hpp"
#include "RootPane.hpp"
#include "WindowToolkit.hpp"
#include "windowImpl/WindowFlags.hpp"
#include "ui/UIManager.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Window
//=============================================================================
Window::Windows Window::openWindows;
//-----------------------------------------------------------------------------
void Window::startMainLoop() {
	getWindowToolkit()->startMainLoop();
}
//-----------------------------------------------------------------------------
AWindowImpl::Ptr Window::getWindowImpl() const {
	return windowImpl;
}
//-----------------------------------------------------------------------------
Window::Window(Window::Ptr parentWindow, unsigned int flags) : parentWindow(parentWindow) {
	rootPane = components::RootPane::create();
	WindowPtr parent = getParentWindow();
	if (parent) {
		windowImpl =
			getWindowToolkit()->createWindowImpl(parent->getWindowImpl());
	} else  {
		windowImpl =
			getWindowToolkit()->createWindowImpl();
	}
	SAMBAG_ASSERT(windowImpl);
    if (flags>0) {
        windowImpl->setFlags(flags);
    } else {
        windowImpl->setFlag(WindowFlags::WND_FRAMED, false);
	}
    windowImpl->setRootPane(rootPane);
	//add(rootPane); do not call in constructor! -> getPtr() returns NULL
}
//-----------------------------------------------------------------------------
Window::~Window() {
}
//-----------------------------------------------------------------------------
Window::Window(AWindowImpl::Ptr windowImpl, Window::Ptr parentWindow) :
    windowImpl(windowImpl), parentWindow(parentWindow)
{
	SAMBAG_ASSERT(windowImpl);
	rootPane = components::RootPane::create();
	windowImpl->setRootPane(rootPane);
	setBounds(windowImpl->getBounds());
}
//-----------------------------------------------------------------------------
void Window::initWindow() {
	add(rootPane);
	WindowPtr parent = getParentWindow();
	if (parent) {
		parent->addTrackedOnCloseEventListener(
			boost::bind(&Window::onParentClose, this, _1, _2),
			getPtr()
		);
	}	
	addTrackedOnCloseEventListener(
		boost::bind(&Window::onWindowImplClose, this, _1, _2),
		getPtr()
	);
	addTrackedOnOpenEventListener(
		boost::bind(&Window::onWindowImplOpen, this, _1, _2),
		getPtr()
	);
	windowImpl->EventSender<OnBoundsChanged>::addTrackedEventListener(
			boost::bind(&Window::onBoundsChanged, this, _1, _2),
			getPtr()
	);
}
//-----------------------------------------------------------------------------
void Window::onParentClose(void *src, const OnCloseEvent &ev) {
	if (windowImpl)
		close();
}
//-----------------------------------------------------------------------------
void Window::onWindowImplClose(void *src, const OnCloseEvent &ev) {
	try {
        ui::UIManager &m = ui::getUIManager();
        m.uninstallLookAndFeel(getRootPane());
    } catch(...) {
    }
}
//-----------------------------------------------------------------------------
void Window::onWindowImplOpen(void *src, const OnOpenEvent &ev) {
	setEnabled(Super::isEnabled());
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
void Window::setWindowSize(const Dimension &d) {
	Rectangle neu = windowImpl->getBounds();
	neu.setWidth(d.width());
	neu.setHeight(d.height());
	setWindowBounds(neu);
}
//-----------------------------------------------------------------------------
void Window::setWindowLocation(const Point2D &p) {
	Rectangle neu = windowImpl->getBounds();
	neu.translate(p);
	setWindowBounds(neu);
}
//-----------------------------------------------------------------------------
bool Window::isVisible() const {
	if (!windowImpl)
		return false;
	return windowImpl->isVisible();
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
	if (windowImpl->getFlag(WindowFlags::WND_NESTED)) {
		return windowImpl->getHostBounds();
	}
	return windowImpl->getBounds();
}
//-----------------------------------------------------------------------------
void Window::open() {
	if (isVisible())
		return;
	windowImpl->open();
	getWindowToolkit()->holdWindowPtr(getPtr());
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		openWindows.insert(getPtr());
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void Window::close() {
	if (!isVisible())
		return;
	windowImpl->close();
    
    getWindowImpl()->EventSender<OnCloseEvent>::
        notifyListeners(windowImpl.get(), OnCloseEvent());
    
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		openWindows.erase(getPtr());
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
/*void _invalidateWindow(AWindowImpl::WPtr _win, Rectangle area) {
	AWindowImpl::Ptr windowImpl = _win.lock();
	if (windowImpl) {
		windowImpl->invalidateWindow(area);
	}
}*/
//-----------------------------------------------------------------------------
void Window::invalidateWindow(const Rectangle &area) {
	windowImpl->invalidateWindow(area);
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

//-----------------------------------------------------------------------------
Window::OnOpenEventSender::Connection
Window::addOnOpenEventListener(
		const Window::OnOpenEventSender::EventFunction &f
)
{
	return windowImpl->EventSender<OnOpenEvent>::addEventListener(f);
}
//-----------------------------------------------------------------------------
Window::OnOpenEventSender::Connection
Window::addTrackedOnOpenEventListener(
		const Window::OnOpenEventSender::EventFunction &f,
		Window::AnyWPtr wPtr
)
{
	return
		windowImpl->EventSender<OnOpenEvent>::addTrackedEventListener(f, wPtr);
}
//-----------------------------------------------------------------------------
Window::WindwowMouseEvent::Connection
Window::addWindowMouseEventListener(const Window::WindwowMouseEvent::EventFunction &f) 
{
	EventSender<events::MouseEvent> *sender =
		windowImpl->getMouseEventCreator();
	if (!sender) {
		SAMBAG_WARN("WindowImpl EventSender==NULL");
		return WindwowMouseEvent::Connection();
	}
	return sender->addEventListener(f);
}
//-----------------------------------------------------------------------------
Window::WindwowMouseEvent::Connection
Window::addTrackedWindowMouseEventListener
(const Window::WindwowMouseEvent::EventFunction &f, Window::AnyWPtr ptr )
{
	EventSender<events::MouseEvent> *sender =
		windowImpl->getMouseEventCreator();
	if (!sender) {
		SAMBAG_WARN("WindowImpl EventSender==NULL");
		return WindwowMouseEvent::Connection();
	}
	return sender->addTrackedEventListener(f, ptr);
}
//-----------------------------------------------------------------------------
void Window::setDefaultCloseOperation(CloseOperation op) {
	if (op==EXIT_ON_CLOSE) {
		windowImpl->setFlag(WindowFlags::EXIT_ON_CLOSE, true);
	} else {
		windowImpl->setFlag(WindowFlags::EXIT_ON_CLOSE, false);
	}
}
//-----------------------------------------------------------------------------
Window::CloseOperation Window::getDefaultCloseOperation() const {
	if (windowImpl->getFlag(WindowFlags::EXIT_ON_CLOSE)) {
		return EXIT_ON_CLOSE;
	}
	return DISPOSE_ON_CLOSE;
}
//-----------------------------------------------------------------------------
void Window::setEnabled(bool b) {
	windowImpl->setEnabled(b);
	Super::setEnabled(b);
}
//-----------------------------------------------------------------------------
void Window::positionWindow(Window::Ptr win) {
	if (!win) {
		win = getParentWindow();
	}
	if (!win) {
		return;
	}
	Point2D p = win->getWindowLocation();
	p.x( p.x() + (win->getWidth()  - getWidth())  / 2. );
	p.y( p.y() + (win->getHeight() - getHeight()) / 2. );
	setWindowLocation(p);
}
//-----------------------------------------------------------------------------
com::ThreadId Window::getThreadId() const {
    return windowImpl->getThreadId();
}
}}} // namespace(s)
