/*
 * WindowTestToolkit.cpp
 *
 *  Created on: Mon Jun 18 15:09:40 2012
 *      Author: Johannes Unger
 */

#include "WindowTestToolkit.hpp"
#include "TestWindow.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class WindowTestToolkit
//=============================================================================
//-----------------------------------------------------------------------------
AWindowImplPtr WindowTestToolkit::createWindowImpl(AWindowImplPtr parent) const {
	return TestWindow::create();
}
//-----------------------------------------------------------------------------
Dimension WindowTestToolkit::getScreenSize() const {
	return Dimension(1024, 768);
}
#ifndef DISCO_USE_WIN32
#ifndef DISCO_USE_X11
#ifndef DISCO_USE_CARBON
#ifndef DISCO_USE_COCOA
// .. add other
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	typedef Loki::SingletonHolder<WindowTestToolkit> FactoryHolder;
	return &FactoryHolder::Instance();
}
#endif
#endif
#endif
#endif
}}} // namespace(s)
