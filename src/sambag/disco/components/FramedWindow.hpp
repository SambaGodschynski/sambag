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
	FramedWindow(Window::Ptr parent=Window::Ptr());
    //-------------------------------------------------------------------------
	FramedWindow(AWindowImpl::Ptr windowImpl, Window::Ptr parentWindow=WindowPtr());
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_WINDOW_CREATOR(FramedWindow)
	//-------------------------------------------------------------------------
	static Ptr create(AWindowImpl::Ptr impl, Window::Ptr parentWindow=WindowPtr())
    {
		Ptr res(new FramedWindow(impl, parentWindow));
		res->self = res;
		res->initWindow();
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void setTitle(const std::string &title);
	//-------------------------------------------------------------------------
	virtual std::string getTitle() const;
}; // FramedWindow
}}} // namespace(s)

#endif /* SAMBAG_FRAMEDWINDOW_H */
