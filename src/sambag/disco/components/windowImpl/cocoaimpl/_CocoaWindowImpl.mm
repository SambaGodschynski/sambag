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
//=============================================================================
@interface DiscoWindow : NSWindow
{}
@end
@implementation DiscoWindow
@end

//=============================================================================
@interface DiscoWindowDelegate : NSObject<NSWindowDelegate>
{
	Master *master;
}
- (void)windowDidResize:(NSNotification *)notification;
- (void)setMaster:(Master*) theMaster;
@end

@implementation DiscoWindowDelegate
- (void)windowDidResize:(NSNotification *)notification {
	master->__windowDidResized();
}
- (void)setMaster:(Master*) theMaster{
	master = theMaster;
}
@end

//=============================================================================
@interface DiscoView : NSView
{
	Master *master;
}
- (BOOL)acceptsFirstResponder;
- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseUp:(NSEvent *)theEvent;
- (void)mouseMoved:(NSEvent *)theEvent;
- (void)mouseDragged:(NSEvent *)theEvent;
- (BOOL)acceptsFirstResponder;
- (void)drawRect:(NSRect)rect;
- (void)setMaster:(Master*) theMaster;
- (NSPoint)flipPoint:(NSPoint)p;
- (NSRect)flipRect:(NSRect)r;
- (NSPoint)getMouseLocation:(NSEvent *)theEvent;
- (int)getMouseBtn:(NSEvent*) ev;
@end


@implementation DiscoView

- (NSRect)flipRect:(NSRect)r {
	NSRect frame = [self frame];
	r.origin.y = frame.size.height - r.origin.y;
	return r;
}

- (NSPoint)flipPoint:(NSPoint)p {
	NSRect frame = [self frame];
	p.y = frame.size.height - p.y;
	return p;
}
- (NSPoint)getMouseLocation:(NSEvent *)theEvent {
	NSPoint p = [self convertPoint: [theEvent locationInWindow] fromView: self];
	return [self flipPoint: p];
}
- (BOOL)acceptsFirstResponder {
    return YES;
}
- (int)getMouseBtn:(NSEvent*) ev {
	int btn = 0;
	switch ([ev type]) {
		case NSLeftMouseDown: btn = 1; break;
		case NSRightMouseDown: btn = 2; break;
		case NSLeftMouseUp: btn = 1; break;
		case NSRightMouseUp: btn = 2; break;
		default: break;
	}
	return btn;
}
- (void)mouseDown:(NSEvent *)theEvent {
	int btn = [self getMouseBtn: theEvent];
	NSPoint p = [self getMouseLocation: theEvent];
	master->__handleMouseButtonPressEvent(p.x, p.y, btn);
}
- (void)mouseUp:(NSEvent *)theEvent {
    int btn = [self getMouseBtn: theEvent];
	NSPoint p = [self getMouseLocation: theEvent];
	master->__handleMouseButtonReleaseEvent(p.x, p.y, btn);
}
- (void)mouseMoved:(NSEvent *)theEvent {
	NSPoint p = [self getMouseLocation: theEvent];
	master->__handleMouseMotionEvent(p.x, p.y);
}
- (void)mouseDragged:(NSEvent *)theEvent{
	NSPoint p = [self getMouseLocation: theEvent];
	master->__handleMouseMotionEvent(p.x, p.y);
}
- (void)drawRect:(NSRect)rect {
	void *gc = [[[self window] graphicsContext] graphicsPort];
	master->__processDraw(
	    (CGContextRef)gc,
		 rect.origin.x, 
		 rect.origin.y, 
		 rect.size.width, 
		 rect.size.height
	);
}
- (void)setMaster:(Master*) theMaster {
	master = theMaster;
}
@end


namespace sambag { namespace disco { namespace components {
namespace {
DiscoWindow * getDiscoWindow(const Master &m) {
	DiscoWindow *res = (DiscoWindow*)m.getRawDiscoWindow();
	//assert(res);
	return res;
}
DiscoView * getDiscoView(const Master &m) {
	DiscoView *res = (DiscoView*)m.getRawDiscoView();
	//assert(res);
	return res;
}
} // namepsace(s)
//=================================================================================
// @class _CocoaWindowImpl
//=================================================================================
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::startMainApp() {
	[NSApplication sharedApplication];
	[NSApp run];
}
//---------------------------------------------------------------------------------
_CocoaWindowImpl::_CocoaWindowImpl() : window(NULL), view(NULL)
{
}
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::invalidateWindow(double x, double y, double w, double h) {
	DiscoView *view = getDiscoView(*this);
	if (!view) {
		return; // happens sometimes (maybe while creating)
	}
	[view setNeedsDisplay:YES];
}

//---------------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow(int x, int y, int w, int h) {
	ap = AutoReleasePool();
	NSRect frame = NSMakeRect(x,y,w,h);
	int options = NSTitledWindowMask | 
	              NSClosableWindowMask | 
	              NSMiniaturizableWindowMask | 
	              NSResizableWindowMask;
	
	NSWindow* window  = [[[DiscoWindow alloc] initWithContentRect:frame
						  styleMask: options
						  backing:NSBackingStoreBuffered
						  defer:NO] autorelease];
	
	[window setBackgroundColor:[NSColor blueColor]];
	[window makeKeyAndOrderFront:window];
	[window setAcceptsMouseMovedEvents:YES];
	
	// add view
	DiscoView *view = [[DiscoView alloc] initWithFrame: NSMakeRect(0,0,w,h)];
	[[window contentView] addSubview:view];
	[view setMaster: this];
	
	NSTrackingArea *trackingArea = [[[NSTrackingArea alloc] initWithRect:[view frame]
								options:NSTrackingMouseEnteredAndExited | 
									    NSTrackingMouseMoved | 
									    NSTrackingInVisibleRect |
									    NSTrackingActiveAlways
							    owner:view
								userInfo:nil] autorelease];
	
	[view addTrackingArea:trackingArea];
	
	// assign raw pointer
	this->window = window;
	this->view = view;
	
	// set delegate
	DiscoWindowDelegate *delegate = [DiscoWindowDelegate alloc];
	[delegate setMaster: this];
	[window setDelegate:delegate];
	
}
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::setBounds(int x, int y, int w, int h) {
	DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
	NSRect frame = NSMakeRect(x, y, w, h);
	[window setFrame:frame display:YES animate:NO];
	// view
	frame = [window contentRectForFrameRect:[window frame]];
	[view setFrameSize: frame.size];
}
//---------------------------------------------------------------------------------
void _CocoaWindowImpl::__windowDidResized() {
	DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
	NSRect frame = [window contentRectForFrameRect:[window frame]];
	[view setFrameSize: frame.size];
	__boundsChanged(frame.origin.x, 
				  frame.origin.y, 
				  frame.size.width, 
				  frame.size.height);
}
}}} //namespace(s)

#endif //DISCO_USE_COCOA