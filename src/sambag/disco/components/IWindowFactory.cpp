/*
 * IWindowFactory.cpp
 *
 *  Created on: Thu Jun  7 10:44:45 2012
 *      Author: Johannes Unger
 */

#include "IWindowFactory.hpp"

namespace sambag { namespace disco { namespace components {
namespace {
	IWindowFactory * currFactory = NULL;
}
//=============================================================================
//  Class IWindowFactory
//=============================================================================
//-----------------------------------------------------------------------------
IWindowFactory * getWindowFactory() {
	if (currFactory)
		return currFactory;
	return _getWindowFactoryImpl();
}
//-----------------------------------------------------------------------------
void setWindowFactory(IWindowFactory *wf) {
	currFactory = wf;
}
}}} // namespace(s)
