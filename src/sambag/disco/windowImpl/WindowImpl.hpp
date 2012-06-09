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
#include "AWindow.hpp"
#include <sambag/disco/Forward.hpp>
#include <sambag/disco/components/RootPane.hpp>

namespace sambag { namespace disco {
//=============================================================================
/**
  * @class WindowImpl.
  */
template <class WindowBase>
class WindowImpl : public WindowBase, public AWindow {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<WindowImpl> Ptr;
protected:
	//-------------------------------------------------------------------------
	sambag::disco::IImageSurface::Ptr bff;
	//-------------------------------------------------------------------------
	components::RootPanePtr rootPane;
	//-------------------------------------------------------------------------
	WindowImpl() : X11WindowImpl() {
	}
	//-------------------------------------------------------------------------
	virtual void processDraw();
	//-------------------------------------------------------------------------
	virtual void onCreated();
	//-------------------------------------------------------------------------
	virtual void onDestroyed();
private:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
	//-------------------------------------------------------------------------
	components::events::MouseEventCreator::Ptr mec;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root) {
		rootPane = root;
	}
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr parent) {

	}
	//-------------------------------------------------------------------------
	virtual void setFramed(bool b) {
		WindowBase::setFramed(b);
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
		needUpdate = true;
	}
	//-------------------------------------------------------------------------
	virtual void boundsUpdated();
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &r) {
		X11WindowImpl::setBounds(r);
	}
	//-------------------------------------------------------------------------
	Rectangle getBounds() const {
		return X11WindowImpl::getBounds();
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new WindowImpl<WindowBase>());
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void open() {
		// init surface
		X11WindowImpl::open();
	}
	//-------------------------------------------------------------------------
	virtual void close() {
		rootPane->setSurface(ISurface::Ptr());
		mec.reset();
		X11WindowImpl::close();
	}
	//--------------------------------------------------------------------------
	virtual void setTitle(const std::string &title) {
		X11WindowImpl::setTitle(title);
	}
}; // X11Window
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class WindowBase>
void WindowImpl<WindowBase>::onCreated() {
	using namespace components;
	// create offbuffer
	Dimension dim = X11WindowImpl::getBounds().getDimension();
	bff = sambag::disco::
			getDiscoFactory()->createImageSurface(dim.width(), dim.height());
	// create root pane
	rootPane->setSurface(bff);
	rootPane->setBounds(X11WindowImpl::getBounds());
	// create mousevent creator
	mec = events::MouseEventCreator::create(rootPane);
	update();
}
//-----------------------------------------------------------------------------
template <class WindowBase>
void WindowImpl<WindowBase>::onDestroyed() {
}
//-----------------------------------------------------------------------------
template <class WindowBase>
void WindowImpl<WindowBase>::processDraw() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	RootPane::Ptr root = rootPane;
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(WindowBase::getSurface());
	if (needUpdate) {
		root->draw(root->getDrawContext());
		needUpdate=false;
		return;
	}
	RedrawManager::currentManager(root)->drawDirtyRegions();
	cn->drawSurface(bff);
}
//-----------------------------------------------------------------------------
template <class WindowBase>
void WindowImpl<WindowBase>::boundsUpdated() {
	using namespace components;
	// create offbuffer
	Dimension dim = X11WindowImpl::getBounds().getDimension();
	if (dim == rootPane->getSize())
		return;
	std::cout<<dim<<std::endl;
	bff = sambag::disco::
			getDiscoFactory()->createImageSurface(dim.width(), dim.height());
	// create root pane
	rootPane->setSurface(bff);
	rootPane->setSize(dim);
	rootPane->validate();
	RedrawManager::currentManager(rootPane)->
			markCompletelyDirty(rootPane);
	update();
}
}} // namespace(s)
#endif /* TWINDOW_HPP_ */
