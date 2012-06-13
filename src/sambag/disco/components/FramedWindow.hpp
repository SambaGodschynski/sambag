/*
 * FramedWindow.hpp
 *
 *  Created on: Mon Jun 11 13:08:01 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_FRAMEDWINDOW_H
#define SAMBAG_FRAMEDWINDOW_H

#include <boost/shared_ptr.hpp>
#include "Window.hpp"
#include <string>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class FramedWindow.
  */
class FramedWindow : public Window {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<FramedWindow> Ptr;
protected:
	FramedWindow(Window::Ptr parent);
private:
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<FramedWindow>(Window::getPtr());
	}
	//-------------------------------------------------------------------------
	static Ptr create(Window::Ptr parent = Window::Ptr()) {
		Ptr neu(new FramedWindow(parent));
		neu->self = neu;
		neu->initWindow();
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const;
}; // FramedWindow
}}} // namespace(s)

#endif /* SAMBAG_FRAMEDWINDOW_H */
