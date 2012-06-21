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
AWindowPtr WindowTestToolkit::createWindowImpl() const {
	return TestWindow::create();
}
}}} // namespace(s)
