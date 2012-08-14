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
	AWindowImplPtr parent;
	//-------------------------------------------------------------------------
	components::RootPanePtr rootPane;
	//-------------------------------------------------------------------------
	WindowImpl() {}
	//-------------------------------------------------------------------------
	virtual void processDraw();
	//-------------------------------------------------------------------------
	virtual void onCreated();
	//-------------------------------------------------------------------------
	virtual void onDestroy();
private:
	//-------------------------------------------------------------------------
	components::events::MouseEventCreator::Ptr mec;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	virtual void invalidateWindow(const Rectangle &area = NULL_RECTANGLE);
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root) {
		rootPane = root;
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
	static Ptr create(AWindowImplPtr parent) {
		Ptr res(new WindowImpl<ConcreteWindowImpl, DrawPolicy>());
		res->ConcreteWindowImpl::self = res;
		res->parent = parent;
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void open() {
		// init surface
		ConcreteWindowImpl::open(parent);
	}
	//-------------------------------------------------------------------------
	virtual void close() {
		ConcreteWindowImpl::close();
	}
	//--------------------------------------------------------------------------
	virtual void setTitle(const std::string &title) {
		ConcreteWindowImpl::setTitle(title);
	}
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const {
		return ConcreteWindowImpl::getTitle();
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
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::onCreated() {
	using namespace components;
	rootPane->setSize(ConcreteWindowImpl::getBounds().getDimension());
	DrawPolicy::init(rootPane, ConcreteWindowImpl::surface);
	rootPane->validate();
	// create mousevent creator
	mec = events::MouseEventCreator::create(rootPane);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::onDestroy() {
	using namespace sambag::com::events;
	rootPane->setSurface(ISurface::Ptr());
	mec.reset();
	EventSender<OnCloseEvent>::notifyListeners (
		this,
		OnCloseEvent()
	);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::processDraw() {
	DrawPolicy::processDraw(ConcreteWindowImpl::surface);
}
//-----------------------------------------------------------------------------
template <class ConcreteWindowImpl, class DrawPolicy>
void WindowImpl<ConcreteWindowImpl, DrawPolicy>::boundsUpdated() {
	using namespace components;
	// create offbuffer
	Dimension dim = ConcreteWindowImpl::getBounds().getDimension();
	if (dim != rootPane->getSize()) {
		rootPane->setSize(dim);
		DrawPolicy::reinit(rootPane, ConcreteWindowImpl::surface);
		rootPane->invalidate();
	}
	EventSender<OnBoundsChanged>::notifyListeners(this, OnBoundsChanged(dim));
}
}}} // namespace(s)
#endif /* TWINDOW_HPP_ */
