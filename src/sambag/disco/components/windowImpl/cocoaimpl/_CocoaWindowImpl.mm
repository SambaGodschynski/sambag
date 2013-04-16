/*
 * _CocoaWindowImpl.mm
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#import "_CocoaWindowImpl.h"
#import <Cocoa/Cocoa.h>
#import "AutoReleasePool.h"
#import <objc/runtime.h>
#import <objc/message.h>
#import <iostream>

static Class viewClass = 0;

//------------------------------------------------------------------------------------
static Class _generateUniqueClass (NSMutableString* className, Class baseClass)
{
	NSString* _className = [NSString stringWithString:className];
	NSInteger iteration = 0;
	id cl = nil;
	while ((cl = objc_lookUpClass ([className UTF8String])) != nil)
	{
		iteration++;
		[className setString:[NSString stringWithFormat:@"%@_%d", _className, iteration]];
	}
	Class resClass = objc_allocateClassPair (baseClass, [className UTF8String], 0);
	return resClass;
}


static Class _initViewClass() {
	[NSApplication sharedApplication];
	AutoReleasePool ap;
	NSMutableString * viewClassName = 
		[[[NSMutableString alloc] initWithString:@"DISCO_NSVIEW"] autorelease ];
	Class res = _generateUniqueClass(viewClassName, [NSView class]);
	return res;
}

namespace sambag { namespace disco { namespace components {
//=================================================================================
// @class _CocoaWindowImpl
//=================================================================================
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::startMainApp() {
	AutoReleasePool ap;
	
	//AppDelegate *appDelegate = [[AppDelegate alloc] init];
	//[NSApp setDelegate:appDelegate];
	[NSApp run];
}
//---------------------------------------------------------------------------------
_CocoaWindowImpl::_CocoaWindowImpl(){
	if (viewClass == 0) {
		viewClass = _initViewClass();
	}
}
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow() {
	NSRect frame = NSMakeRect(0, 0, 200, 200);
	NSWindow* window  = [[[NSWindow alloc] initWithContentRect:frame
						  styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
						  backing:NSBackingStoreBuffered
						  defer:NO] autorelease];
	[window setBackgroundColor:[NSColor blueColor]];
	[window makeKeyAndOrderFront:NSApp];
}

}}} //namespace(s)

#endif //DISCO_USE_COCOA