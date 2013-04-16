/*
 * AutoReleasePool.h
 *
 *  Created on: Mon Jul  2 10:50:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COCOA_AUTORELEASE_POOL
#define SAMBAG_COCOA_AUTORELEASE_POOL

#ifdef DISCO_USE_COCOA

@class NSAutoreleasePool;

//=============================================================================
class AutoReleasePool {
//=============================================================================
public:
	AutoReleasePool ();
	~AutoReleasePool ();
protected:
	NSAutoreleasePool* pool;
};

#endif //DISCO_USE_COCOA
#endif //SAMBAG_COCOA_AUTORELEASE_POOL

