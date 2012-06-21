/*
 * X11WindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11WindowToolkit.hpp"
#include "X11Window.hpp"
#include "WindowImpl.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class X11WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
void X11WindowToolkit::startMainLoop() {
	X11WindowImpl::startMainLoop();
}
//-----------------------------------------------------------------------------
AWindowPtr X11WindowToolkit::createWindowImpl() const {
	AWindowPtr res = WindowImpl<X11WindowImpl>::create();
	return res;
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
typedef Loki::SingletonHolder<X11WindowToolkit> X11WindowFactoryHolder;
WindowToolkit * _getWindowToolkitImpl() {
	return &X11WindowFactoryHolder::Instance();
}
}}} // namespace(s)

#endif // DISCO_USE_X11
