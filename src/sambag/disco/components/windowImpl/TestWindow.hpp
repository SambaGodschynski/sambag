/*
 * TestWindow.hpp
 *
 *  Created on: Mon Jun 18 14:48:52 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTWINDOW_H
#define SAMBAG_TESTWINDOW_H

#include <boost/shared_ptr.hpp>
#include "AWindow.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class TestWindow.
  */
class TestWindow : public AWindow {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<TestWindow> Ptr;
protected:
	//-------------------------------------------------------------------------
	TestWindow(){}
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
	static Ptr create() {
		return Ptr(new TestWindow());
	}
	//-------------------------------------------------------------------------
	virtual void open();
	//-------------------------------------------------------------------------
	virtual void close();
	//-------------------------------------------------------------------------
	virtual void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual Rectangle getBounds() const;
	//-------------------------------------------------------------------------
	virtual void setRootPane(components::RootPanePtr root);
	//-------------------------------------------------------------------------
	virtual void setParentWindow(WindowPtr parent);
	//-------------------------------------------------------------------------
	virtual void setFramed(bool b);
	//-------------------------------------------------------------------------
	virtual bool isVisible() const;
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const;
}; // TestWindow
}}} // namespace(s)

#endif /* SAMBAG_TESTWINDOW_H */
