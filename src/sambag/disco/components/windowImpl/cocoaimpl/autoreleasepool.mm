/*
 * AutoReleasePool.h
 *
 *  Created on: Mon Jul  2 10:50:11 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA

#import "AutoReleasePool.h"
#import <Foundation/Foundation.h>
//=============================================================================
// class AutoReleasePool
//=============================================================================
//-----------------------------------------------------------------------------
AutoReleasePool::AutoReleasePool ()
{
	//NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	//_pool = pool;
}

//-----------------------------------------------------------------------------
AutoReleasePool::~AutoReleasePool ()
{
	//NSAutoreleasePool *pool = (NSAutoreleasePool*)_pool;
	//[pool release];
}

#endif // DISCO_USE_COCOA