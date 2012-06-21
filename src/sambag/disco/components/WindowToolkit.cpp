/*
 * WindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:45 2012
 *      Author: Johannes Unger
 */

#include "WindowToolkit.hpp"

namespace sambag { namespace disco { namespace components {
namespace {
	WindowToolkit * currFactory = NULL;
}
//=============================================================================
//  Class WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
WindowToolkit * getWindowToolkit() {
	if (currFactory)
		return currFactory;
	return _getWindowToolkitImpl();
}
//-----------------------------------------------------------------------------
void setWindowFactory(WindowToolkit *wf) {
	currFactory = wf;
}
}}} // namespace(s)
