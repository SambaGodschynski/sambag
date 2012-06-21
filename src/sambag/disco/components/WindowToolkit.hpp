/*
 * WindowToolkit.hpp
 *
 *  Created on: Thu Jun  7 10:44:45 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WINDOWTOOLKIT_H
#define SAMBAG_WINDOWTOOLKIT_H

#include <boost/shared_ptr.hpp>
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class WindowToolkit.
  */
class WindowToolkit {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const = 0;
	//-------------------------------------------------------------------------
	virtual void startMainLoop() = 0;
}; // WindowToolkit
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
extern WindowToolkit * _getWindowToolkitImpl();
//-----------------------------------------------------------------------------
WindowToolkit * getWindowToolkit();
//-----------------------------------------------------------------------------
/**
 * used for testing
 * @param current Windowfactory
 */
void setWindowFactory(WindowToolkit *wf);
}}} // namespace(s)
#endif /* SAMBAG_WINDOWTOOLKIT_H */
