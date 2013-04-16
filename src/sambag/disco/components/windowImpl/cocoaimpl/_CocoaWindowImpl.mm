/*
 * _CocoaWindowImpl.mm
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
//#import "_CocoaWindowImpl.h"
#import <Cocoa/Cocoa.h>
/*
namespace sambag { namespace disco { namespace components {
//=============================================================================
// @class _CocoaWindowImpl
//=============================================================================
void _CocoaWindowImpl::openWindow() {
	NSRect frame = NSMakeRect(100, 100, 200, 200);
	NSUInteger styleMask =    NSBorderlessWindowMask;
	NSRect rect = [NSWindow contentRectForFrameRect:frame styleMask:styleMask];
	NSWindow * window =  [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing: NSBackingStoreBuffered    defer:false];
	[window setBackgroundColor:[NSColor blueColor]];
	[window makeKeyAndOrderFront: window];
}

}}} //namespace(s)*/

#endif //DISCO_USE_COCOA