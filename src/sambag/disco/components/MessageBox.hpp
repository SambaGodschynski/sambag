/*
 * MessageBox.hpp
 *
 *  Created on: Tue Jun 19 21:04:49 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MESSAGEBOX_H
#define SAMBAG_MESSAGEBOX_H

#include <boost/shared_ptr.hpp>
#include "FramedWindow.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class MessageBox.
  */
class MessageBox : public FramedWindow {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<MessageBox> Ptr;
protected:
	//-------------------------------------------------------------------------
	MessageBox(Window::Ptr parent) : FramedWindow(parent) {}
private:
public:
	SAMBAG_STD_WINDOW_CREATOR(MessageBox)
}; // MessageBox
}}} // namespace(s)

#endif /* SAMBAG_MESSAGEBOX_H */
