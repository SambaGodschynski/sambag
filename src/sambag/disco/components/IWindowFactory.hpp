/*
 * IWindowFactory.hpp
 *
 *  Created on: Thu Jun  7 10:44:45 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IWINDOWFACTORY_H
#define SAMBAG_IWINDOWFACTORY_H

#include <boost/shared_ptr.hpp>
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class IWindowFactory.
  */
class IWindowFactory {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const = 0;
	//-------------------------------------------------------------------------
	virtual void startMainLoop() = 0;
}; // IWindowFactory
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
extern IWindowFactory * _getWindowFactoryImpl();
//-----------------------------------------------------------------------------
IWindowFactory * getWindowFactory();
//-----------------------------------------------------------------------------
/**
 * used for testing
 * @param current Windowfactory
 */
void setWindowFactory(IWindowFactory *wf);
}}} // namespace(s)
#endif /* SAMBAG_IWINDOWFACTORY_H */
