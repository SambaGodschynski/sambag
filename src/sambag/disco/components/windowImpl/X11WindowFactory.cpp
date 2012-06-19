/*
 * X11WindowFactory.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11WindowFactory.hpp"
#include "X11Window.hpp"
#include "WindowImpl.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class X11WindowFactory
//=============================================================================
//-----------------------------------------------------------------------------
void X11WindowFactory::startMainLoop() {
	X11WindowImpl::startMainLoop();
}
//-----------------------------------------------------------------------------
AWindowPtr X11WindowFactory::createWindowImpl() const {
	AWindowPtr res = WindowImpl<X11WindowImpl>::create();
	return res;
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
typedef Loki::SingletonHolder<X11WindowFactory> X11WindowFactoryHolder;
IWindowFactory * _getWindowFactoryImpl() {
	return &X11WindowFactoryHolder::Instance();
}
}}} // namespace(s)

#endif // DISCO_USE_X11
