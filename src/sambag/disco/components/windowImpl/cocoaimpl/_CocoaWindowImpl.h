/*
 * _CocoaWindowImpl.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG__COCOAWINDOWIMPL_H
#define SAMBAG__COCOAWINDOWIMPL_H
#ifdef DISCO_USE_COCOA

#include <ApplicationServices/ApplicationServices.h>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/**
 * @class _CocoaWindowImpl.
 * Encapsulated cocoa window implementation.
 */
class _CocoaWindowImpl {
protected:
	//-------------------------------------------------------------------------
	_CocoaWindowImpl();
public:
	//-------------------------------------------------------------------------
	static void startMainApp();
	//-------------------------------------------------------------------------
	void openWindow();
	//-------------------------------------------------------------------------
	virtual void __processDraw(CGContextRef context, int x, int y, int w, int h) = 0;
};

}}}

#endif //DISCO_USE_COCOA
#endif //SAMBAG__COCOAWINDOWIMPL_H
