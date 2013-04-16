/*
 * _CocoaWindowImpl.hpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG__COCOAWINDOWIMPL_H
#define SAMBAG__COCOAWINDOWIMPL_H

#ifdef DISCO_USE_COCOA
namespace sambag { namespace disco { namespace components {
//=============================================================================
/**
 * @class _CocoaWindowImpl.
 * Encapsulated cocoa window implementation.
 */
class _CocoaWindowImpl {
	//-------------------------------------------------------------------------
	void openWindow();
};

}}}

#endif //DISCO_USE_COCOA
#endif //SAMBAG__COCOAWINDOWIMPL_H
