/*
 * X11WindowFactory.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_X11

#include "X11WindowFactory.hpp"
#include "X11Window.hpp"

namespace sambag { namespace disco {
//=============================================================================
//  Class X11WindowFactory
//=============================================================================
//-----------------------------------------------------------------------------
AWindowPtr X11WindowFactory::createWindow( AWindowPtr parent ) const {
	AWindowPtr res = X11Window<AWindow>::create(parent, false);
	return res;
}
//-----------------------------------------------------------------------------
AFramedWindowPtr
X11WindowFactory::createFramedWindow( AWindowPtr parent ) const
{
	AFramedWindowPtr res = X11Window<AFramedWindow>::create(parent, true);
	return res;
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
typedef Loki::SingletonHolder<X11WindowFactory> X11WindowFactoryHolder;
IWindowFactory * getWindowFactory() {
	return &X11WindowFactoryHolder::Instance();
}
}} // namespace(s)

#endif // DISCO_USE_X11
