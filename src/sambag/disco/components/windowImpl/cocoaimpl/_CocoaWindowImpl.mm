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

typedef sambag::disco::components::_CocoaWindowImpl Master;

@interface TestView : NSWindow
{
	
}
- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseMoved:(NSEvent *)theEvent;
- (BOOL)acceptsFirstResponder;
- (void)update;
@end


@implementation TestView
- (BOOL)acceptsFirstResponder {
    return YES;
}
- (void)mouseDown:(NSEvent *)theEvent {
    NSLog(@"mouseDown event detected!");
}
- (void)mouseMoved:(NSEvent *)theEvent {
    NSLog(@"mouseMoved event detected!");
}
- (void)update {
}
@end

@interface TestNSView : NSView
{
	Master *parent;
}
- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseMoved:(NSEvent *)theEvent;
- (BOOL)acceptsFirstResponder;
- (void)drawRect:(NSRect)rect;
- (void)setParent:(Master*) theParent;
@end


@implementation TestNSView
- (BOOL)acceptsFirstResponder {
    return YES;
}
- (void)mouseDown:(NSEvent *)theEvent {
    NSLog(@"mouseDown event detected!");
}
- (void)mouseMoved:(NSEvent *)theEvent {
    NSLog(@"mouseMoved event detected!");
}
- (void)drawRect:(NSRect)rect {
	void *gc = [[[self window] graphicsContext] graphicsPort];
	parent->__processDraw((CGContextRef)gc,0,0,200,200);
	
}
- (void)setParent:(Master*) theParent{
	parent = theParent;
}
@end



static Class viewClass = 0;
//------------------------------------------------------------------------------------
static void _onMouseDown(id self, SEL _cmd, NSEvent *ev) {
	/*std::cout<<"1";
	return YES;*/
}
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
	NSMutableString * viewClassName = 
		[[[NSMutableString alloc] initWithString:@"DISCO_NSVIEW"] autorelease ];
	Class res = _generateUniqueClass(viewClassName, [NSView class]);
	BOOL suc;
	suc = class_addMethod(res, @selector(onMouseDown:), IMP(_onMouseDown), "B@:@:^:");
	objc_registerClassPair(res);
	std::cout<<"NSVIEW created"<<std::endl;
	return res;
}

namespace sambag { namespace disco { namespace components {
//=================================================================================
// @class _CocoaWindowImpl
//=================================================================================
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::startMainApp() {
	[NSApplication sharedApplication];
	AutoReleasePool ap;
	//AppDelegate *appDelegate = [[AppDelegate alloc] init];
	//[NSApp setDelegate:appDelegate];
	[NSApp run];
}
//---------------------------------------------------------------------------------
_CocoaWindowImpl::_CocoaWindowImpl(){
	/*if (viewClass == 0) {
		viewClass = _initViewClass();
	}*/
}
	
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow() {
	//AutoReleasePool ap;
	NSRect frame = NSMakeRect(0, 0, 200, 200);
	NSWindow* window  = [[[TestView alloc] initWithContentRect:frame
						  styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
						  backing:NSBackingStoreBuffered
						  defer:NO] autorelease];
	[window setBackgroundColor:[NSColor blueColor]];
	[window makeKeyAndOrderFront:window];
	[window setAcceptsMouseMovedEvents:YES];
	
	TestNSView *view = [[TestNSView alloc] initWithFrame: frame];
	[[window contentView] addSubview:view];
	[view setParent:this];
}

}}} //namespace(s)

#endif //DISCO_USE_COCOA