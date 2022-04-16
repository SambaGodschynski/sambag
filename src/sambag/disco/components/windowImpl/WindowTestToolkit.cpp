/*
 * WindowTestToolkit.cpp
 *
 *  Created on: Mon Jun 18 15:09:40 2012
 *      Author: Johannes Unger
 */

#include "WindowTestToolkit.hpp"

namespace sambag { namespace disco { namespace components {
#ifndef DISCO_USE_WIN32
#ifndef DISCO_USE_X11
#ifndef DISCO_USE_CARBON
#ifndef DISCO_USE_COCOA
// .. add other
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	typedef Loki::SingletonHolder< WindowTestToolkit<> > FactoryHolder;
	return &FactoryHolder::Instance();
}
#endif
#endif
#endif
#endif
}}} // namespace(s)
