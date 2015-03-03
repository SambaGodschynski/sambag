/*
 * FramedWindow.cpp
 *
 *  Created on: Mon Jun 11 13:08:01 2012
 *      Author: Johannes Unger
 */

#include "FramedWindow.hpp"
namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class FramedWindow
//=============================================================================
//-----------------------------------------------------------------------------
FramedWindow::FramedWindow(Window::Ptr parent) : Window(parent) {
	windowImpl->setFlag(WindowFlags::WND_FRAMED, true);
	windowImpl->setFlag(WindowFlags::WND_RESIZEABLE, true);
}
//-----------------------------------------------------------------------------
FramedWindow::FramedWindow(AWindowImpl::Ptr windowImpl, Window::Ptr parentWindow)
    : Window(windowImpl, parentWindow)
{
	windowImpl->setFlag(WindowFlags::WND_FRAMED, true);
	windowImpl->setFlag(WindowFlags::WND_RESIZEABLE, true);
}
//-------------------------------------------------------------------------
void FramedWindow::setTitle(const std::string &title) {
	windowImpl->setTitle(title);
}
//-------------------------------------------------------------------------
std::string FramedWindow::getTitle() const {
	return windowImpl->getTitle();
}
}}} // namespace(s)
