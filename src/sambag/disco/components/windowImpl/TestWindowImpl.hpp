/*
 * TestWindowImpl.hpp
 *
 *  Created on: Mon Jun 18 14:48:52 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTWINDOWIMPL_H
#define SAMBAG_TESTWINDOWIMPL_H

#include <boost/shared_ptr.hpp>
#include "AWindowImpl.hpp"
#include "WindowFlags.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class TestWindow.
  */
class TestWindowImpl : public AWindowImpl, public WindowFlags {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<TestWindowImpl> Ptr;
protected:
	//-------------------------------------------------------------------------
	TestWindowImpl(){}
private:
	//-------------------------------------------------------------------------
	bool isOpen;
	//-------------------------------------------------------------------------
	std::string title;
	//-------------------------------------------------------------------------
	Rectangle bounds;
	//-------------------------------------------------------------------------
	RootPanePtr root;
	//-------------------------------------------------------------------------
	WindowPtr parent;
public:
	//-------------------------------------------------------------------------
	virtual void * getSystemHandle() {
		return NULL;
	}
	//-------------------------------------------------------------------------
	virtual EventSender<events::MouseEvent> * getMouseEventSender() {
		return NULL;
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new TestWindowImpl());
	}
	//-------------------------------------------------------------------------
	/**
	 * AWindow open
	 */
	virtual void open() {
		open(AWindowImplPtr());
	}
	//-------------------------------------------------------------------------
	virtual void open(AWindowImplPtr parent);
	//-------------------------------------------------------------------------
	virtual void close();
	//-------------------------------------------------------------------------
	virtual void invalidateWindow(const Rectangle &area = NULL_RECTANGLE);
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root);
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr parent);
	//-------------------------------------------------------------------------
	virtual void setFlag(WindowFlags::Flag flag, bool val) {
		WindowFlags::setFlag(flag, val);
	}
	//-------------------------------------------------------------------------
	virtual bool getFlag(WindowFlags::Flag flag) const {
		return WindowFlags::getFlag(flag);
	}
	//-------------------------------------------------------------------------
	virtual bool isVisible() const;
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const;
}; // TestWindowImpl
}}} // namespace(s)

#endif /* SAMBAG_TESTWINDOWIMPL_H */
