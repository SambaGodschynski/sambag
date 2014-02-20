/*
 * WindowImpl.hpp
 *
 *  Created on: Jun 8, 2012
 *      Author: samba
 */
#ifndef WINDOWIMPL_HPP_
#define WINDOWIMPL_HPP_

#include <sambag/disco/components/events/MouseEventCreator.hpp>
#include <list>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include "AWindowImpl.hpp"
#include <sambag/disco/components/Forward.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include "BufferedDrawPolicy.hpp"
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>
#include <sambag/com/Common.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/**
  * @class WindowImpl.
  */
template <
	class ConcreteWindowImpl,
	class DrawPolicy = BufferedDrawPolicy
>
class WindowImpl :
	public ConcreteWindowImpl,
	public DrawPolicy,
	public AWindowImpl
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<WindowImpl> Ptr;
protected:
	//-------------------------------------------------------------------------
	AWindowImpl::WPtr _parent;
	//-------------------------------------------------------------------------
	components::RootPanePtr rootPane;
	//-------------------------------------------------------------------------
	WindowImpl() : _thID( com::getThreadId() ) {}
	//-------------------------------------------------------------------------
	virtual void processDraw( sambag::disco::ISurface::Ptr surface );
	//-------------------------------------------------------------------------
	virtual void onCreated();
	//-------------------------------------------------------------------------
	virtual void onDestroy();
private:
    //-------------------------------------------------------------------------
    com::ThreadId _thID;
	//-------------------------------------------------------------------------
	components::events::MouseEventCreator::Ptr mec;
	//-------------------------------------------------------------------------
	void initRootPane();
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	virtual void * getSystemHandle() {
		return ConcreteWindowImpl::getSystemHandle();
	}
	//-------------------------------------------------------------------------
	virtual EventSender<events::MouseEvent> * getMouseEventCreator() {
		return mec.get();
	}
	//-------------------------------------------------------------------------
	virtual void invalidateWindow(const Rectangle &area = NULL_RECTANGLE);
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root) {
		rootPane = root;
		if (isVisible())
			initRootPane();
	}
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr parent) {

	}
	//-------------------------------------------------------------------------
	virtual bool isVisible() const {
		return ConcreteWindowImpl::isVisible();
	}
	//-------------------------------------------------------------------------
	virtual void setFlag(WindowFlags::Flag flag, bool val) {
		return ConcreteWindowImpl::setFlag(flag, val);
	}
	//-------------------------------------------------------------------------
	virtual bool getFlag(WindowFlags::Flag flag) const {
		return ConcreteWindowImpl::getFlag(flag);
	}
	//-------------------------------------------------------------------------
	void handleMouseButtonPressEvent(int x, int y, int buttons) {
		if (!mec)
			return;
		mec->createPressEvent(x,y,buttons);
	}
	//-------------------------------------------------------------------------
	void handleMouseButtonReleaseEvent(int x, int y, int buttons) {
		if (!mec)
			return;
		mec->createReleaseEvent(x,y,buttons);
	}
	//-------------------------------------------------------------------------
	void handleMouseMotionEvent(int x, int y) {
		if (!mec)
			return;
		mec->createMoveEvent(x,y);
	}
	//-------------------------------------------------------------------------
	void handleMouseWheelEvent(int x, int y, float rot) {
		if (!mec)
			return;
		mec->createWheelEvent(x,y,rot);
	}
	//-------------------------------------------------------------------------
	void update() {
		DrawPolicy::update();
	}
	//-------------------------------------------------------------------------
	virtual void boundsUpdated();
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &r) {
		ConcreteWindowImpl::setBounds(r);
	}
	//-------------------------------------------------------------------------
	Rectangle getBounds() const {
		return ConcreteWindowImpl::getBounds();
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getHostBounds() const {
		return ConcreteWindowImpl::getHostBounds();
	}
	//-------------------------------------------------------------------------
	static Ptr create(AWindowImplPtr parent) {
		Ptr res(new WindowImpl<ConcreteWindowImpl, DrawPolicy>());
		res->ConcreteWindowImpl::self = res;
		res->_parent = parent;
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * creates a nested window.
	 */
	static Ptr create(ArbitraryType::Ptr osParent, 
		const Rectangle &area) 
	{
        SAMBAG_LOG_INFO<<"creating nested view: ..., ("<<area<<")";
		Ptr res(new WindowImpl<ConcreteWindowImpl, DrawPolicy>());
		res->ConcreteWindowImpl::initAsNestedWindow(osParent, area);
		res->ConcreteWindowImpl::self = res;
		res->setFlag(WindowFlags::WND_NESTED, true);
        SAMBAG_LOG_INFO<<"creating nested view: SUCCEED";
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void open() {
		// is window nested open/close is deactivated.
		if (getFlag(WindowFlags::WND_NESTED))
			return; 
		// init surface
		SAMBAG_LOG_INFO<<"opening window: ...";
        ConcreteWindowImpl::open(_parent.lock());
        SAMBAG_LOG_INFO<<"opening window: SUCCEED";
        
	}
	//-------------------------------------------------------------------------
	virtual void close() {
        SAMBAG_LOG_INFO<<"closing window: ...";
		ConcreteWindowImpl::close();
        SAMBAG_LOG_INFO<<"closing window: SUCCEED";
	}
	//--------------------------------------------------------------------------
	virtual void setTitle(const std::string &title) {
		ConcreteWindowImpl::setTitle(title);
	}
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const {
		return ConcreteWindowImpl::getTitle();
	}
	//-------------------------------------------------------------------------
	virtual void setEnabled(bool b) {
		ConcreteWindowImpl::setEnabled(b);
	}
	//-------------------------------------------------------------------------
	virtual bool isEnabled() const {
		return ConcreteWindowImpl::isEnabled();
	}
    //-------------------------------------------------------------------------
    com::ThreadId getThreadId() const {
        return _thID;
    }
};
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::invalidateWindow(
	const Rectangle &area) 
{
	ConcreteWindowImpl::invalidateWindow(area);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::initRootPane() {
	using namespace components;
	rootPane->setSize(ConcreteWindowImpl::getBounds().getDimension());
	DrawPolicy::init(rootPane);
	rootPane->validate();
	// create mousevent creator
    events::MouseEventCreator::Ptr proto =
        getWindowToolkit()->getMouseEventCreatorPrototype();
    if (!proto) {
        SAMBAG_THROW(com::exceptions::IllegalStateException,
        "MouseEventCreatorPrototype == NULL");
    }
    mec = proto->clone();
    mec->setRootPane(rootPane);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::onCreated() {
    if (rootPane)
		initRootPane();
	EventSender<OnOpenEvent>::notifyListeners (
		this,
		OnOpenEvent()
	);
    SAMBAG_LOG_INFO<<"  + view created ("<<getBounds()<<")";
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::onDestroy() {
	using namespace sambag::com::events;
    DrawPolicy::close();
	rootPane->setSurface(ISurface::Ptr());
	mec.reset();
	EventSender<OnCloseEvent>::notifyListeners (
		this,
		OnCloseEvent()
	);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::processDraw(
	sambag::disco::ISurface::Ptr surface) 
{
	DrawPolicy::processDraw(surface);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::boundsUpdated() {
	using namespace components;
	Dimension dim = ConcreteWindowImpl::getBounds().getDimension();
	// create offbuffer
	if (rootPane) {
		if (dim != rootPane->getSize()) {
			rootPane->setSize(dim);
			DrawPolicy::reinit(rootPane);
			rootPane->invalidate();
		}
	}
	EventSender<OnBoundsChanged>::notifyListeners(this, OnBoundsChanged(dim));
}
}}} // namespace(s)
#endif /* TWINDOW_HPP_ */
