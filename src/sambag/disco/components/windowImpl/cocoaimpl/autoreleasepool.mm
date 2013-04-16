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
	pool = [[NSAutoreleasePool alloc] init];
}

//-----------------------------------------------------------------------------
AutoReleasePool::~AutoReleasePool ()
{
	[pool release];
}

#endif // DISCO_USE_COCOA