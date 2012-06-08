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

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class IWindowFactory.
  */
class IWindowFactory {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const = 0;
}; // IWindowFactory
///////////////////////////////////////////////////////////////////////////////
extern IWindowFactory * getWindowFactory();
}} // namespace(s)
#endif /* SAMBAG_IWINDOWFACTORY_H */
