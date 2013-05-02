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
#import <stdexcept>
#import <Carbon/Carbon.h>

namespace {
	std::string toString(NSString *str) {
		if (!str) {
			return "";
		}
		return std::string([str UTF8String]);
	}
} // namespace(s)

typedef sambag::disco::components::_CocoaWindowImpl Master;
//=========================================================================
@interface DiscoWindow : NSWindow
{}
@end
@implementation DiscoWindow
@end

//=========================================================================
@interface DiscoWindowDelegate : NSObject<NSWindowDelegate>
{
	Master *master;
}
- (void)windowDidResize:(NSNotification *)notification;
- (void)windowDidMove:(NSNotification *)notification;
- (void)setMaster:(Master*) theMaster;
@end

@implementation DiscoWindowDelegate
- (void)windowDidResize:(NSNotification *)notification {
	master->____windowBoundsChanged();
}
- (void)windowDidMove:(NSNotification *)notification {
	master->____windowBoundsChanged();
}
- (void)setMaster:(Master*) theMaster{
	master = theMaster;
}
@end

//=========================================================================
@interface DiscoView : NSView
{
	Master *master;
}
- (BOOL)acceptsFirstResponder;
- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseUp:(NSEvent *)theEvent;
- (void)rightMouseDown:(NSEvent *)theEvent;
- (void)rightMouseUp:(NSEvent *)theEvent;
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
- (void)rightMouseDown:(NSEvent *)theEvent{
	int btn = [self getMouseBtn: theEvent];
	NSPoint p = [self getMouseLocation: theEvent];
	master->__handleMouseButtonPressEvent(p.x, p.y, btn);
}
- (void)rightMouseUp:(NSEvent *)theEvent{
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
//=============================================================================
// @class _CocoaWindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
_CocoaWindowImpl::_CocoaWindowImpl() : window(NULL), view(NULL)
{
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::invalidateWindow(Number x, Number y, Number w, Number h) {
	DiscoView *view = getDiscoView(*this);
	if (!view) {
		return; // happens sometimes (maybe while creating)
	}
	[view setNeedsDisplay:YES];
}

//-----------------------------------------------------------------------------
namespace {
DiscoView * _initView(_CocoaWindowImpl *caller, const NSRect &frame) {
	DiscoWindow *window = getDiscoWindow(*caller);
	if (!window) {
		return NULL; 
	}
	// create view
	DiscoView *view = [
		[DiscoView alloc] 
				initWithFrame: NSMakeRect(0,0,frame.size.width, frame.size.height)];
	if (!view) {
		throw std::runtime_error("_initView() failed to create view!");
	}
	[[window contentView] addSubview:view];
	[window makeFirstResponder: view];
	[view becomeFirstResponder];
	[view setMaster: caller];
	
	// tracking area
	NSTrackingArea *trackingArea = [[[NSTrackingArea alloc] initWithRect:[view frame]
									 options:NSTrackingMouseEnteredAndExited | 
									 NSTrackingMouseMoved | 
									 NSTrackingInVisibleRect |
									 NSTrackingActiveAlways
									 owner:view
									 userInfo:nil] autorelease];
	
	[view addTrackingArea:trackingArea];
	
	// set delegate
	DiscoWindowDelegate *delegate = [DiscoWindowDelegate alloc];
	[delegate setMaster: caller];
	[window setDelegate:delegate];
	
	return view;
}
} // namespace(s)
//-----------------------------------------------------------------------------
int _CocoaWindowImpl::getWindowStyleMask() const {
	int options = NSTitledWindowMask | 
	NSClosableWindowMask | 
	NSMiniaturizableWindowMask;
	
	if (getFlag(WND_RESIZEABLE)) {
		options |= NSResizableWindowMask;
	}
	
	if ( !getFlag(WND_FRAMED) ) {
		options |= NSBorderlessWindowMask;
		options &= ~NSTitledWindowMask;
		options &= ~NSClosableWindowMask;
		options &= ~NSMiniaturizableWindowMask;
	}
	
	if ( getFlag(WND_NO_SYSTEM_MENU) ) {
		options &= NSClosableWindowMask;
		options &= ~NSMiniaturizableWindowMask;
	}
	return options;
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow(_CocoaWindowImpl *parent, Number x, Number y, Number w, Number h) 
{
	ap = AutoReleasePool();
	NSRect frame = NSMakeRect(x,y,w,h);
	int options = getWindowStyleMask();
	NSWindow* window  = [[[DiscoWindow alloc] initWithContentRect:frame
						  styleMask: options
						  backing:NSBackingStoreBuffered
						  defer:NO] autorelease];
	this->window = window;
	[window setBackgroundColor:[NSColor blueColor]];
	[window makeKeyAndOrderFront:window];
	[window setAcceptsMouseMovedEvents:YES];
	// set parent
	if (parent) {
		DiscoWindow *pWin = getDiscoWindow(*parent);
		if (pWin) {
			[window setParentWindow:pWin];
		}
	}
	NSView * view = _initView(this, frame);
	this->view = view;
}
static pascal OSStatus windowVisibilityBodge (EventHandlerCallRef, EventRef e, void* user)
{
    NSWindow* hostWindow = (NSWindow*) user;
        
    switch (GetEventKind (e))
    {
        case kEventWindowInit:
            [hostWindow display];
            break;
        case kEventWindowShown:
            [hostWindow orderFront: nil];
            break;
        case kEventWindowHidden:
            [hostWindow orderOut: nil];
            break;
    }
        
    return eventNotHandledErr;
}
    
static void attachWindowHidingHooks (void* hostWindowRef, NSWindow* nsWindow)
{
    const EventTypeSpec eventsToCatch[] =
    {
            { kEventClassWindow, kEventWindowInit },
            { kEventClassWindow, kEventWindowShown },
            { kEventClassWindow, kEventWindowHidden }
        };
        
    EventHandlerRef ref;
    InstallWindowEventHandler ((WindowRef) hostWindowRef,
                                NewEventHandlerUPP (windowVisibilityBodge),
                                GetEventTypeCount (eventsToCatch), eventsToCatch,
                                (void*) nsWindow, &ref);
        
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openNested(WindowRef parent, 
	Number x, Number y, Number w, Number h) 
{
    //ap = AutoReleasePool();
	NSRect frame = NSMakeRect(x,y,w,h);
	NSWindow *window = [[NSWindow alloc] initWithWindowRef:parent];
	if (!window) {
		throw std::runtime_error("openNested() failed to create window.");
	}
    [window retain];
    [window setCanHide: YES];
    [window setReleasedWhenClosed: YES];
    [window setIsVisible:YES];
    
    attachWindowHidingHooks(parent, window);
    
    int options = getWindowStyleMask();
	NSWindow* pluginWindow  = [[[DiscoWindow alloc] initWithContentRect:frame
                                                        styleMask: options
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO] autorelease];

	
    
    this->window = pluginWindow;
    [window addChildWindow:pluginWindow ordered:NSWindowAbove];
    [window orderFront: nil];


	this->window = pluginWindow;
	NSView * view = _initView(this, frame);
	// assign raw pointer
	this->view = view;
}
//-----------------------------------------------------------------------------
WindowRef _CocoaWindowImpl::getWindowRef() const {
	NSWindow *win = getDiscoWindow(*this);
	if (!win) {
		return NULL;
	}
	return (WindowRef)[win windowRef];
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::closeWindow() {
	DiscoWindow *window = getDiscoWindow(*this);
	if (!window) {
		return;
	}
	[window close];
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::setBounds(Number x, Number y, Number w, Number h) {
	DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
	NSRect frame = NSMakeRect(x, y, w, h);
	[window setFrame:frame display:YES animate:NO];
	// view
	frame = [window contentRectForFrameRect:[window frame]];
	[view setFrameSize: frame.size];
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::____windowBoundsChanged() {
	DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
	NSRect frame = [window contentRectForFrameRect:[window frame]];
	[view setFrameSize: frame.size];
	__boundsChanged(frame.origin.x, 
				  frame.origin.y, 
				  frame.size.width, 
				  frame.size.height);
}
//-----------------------------------------------------------------------------
std::string _CocoaWindowImpl::getTitle() const {
	NSWindow *window = getDiscoWindow(*this);
	if (!window) {
		return "";
	}
	return toString( [window title] );
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::setTitle(const std::string &str) {
	NSWindow *window = getDiscoWindow(*this);
	if (!window) {
		return;
	}
	[window setTitle: [NSString stringWithUTF8String:str.c_str()]];
}
//=============================================================================
// class _CocoaToolkitImpl 
//=============================================================================
//-----------------------------------------------------------------------------
void _CocoaToolkitImpl::initMainApp() {
    assert( NSApplicationLoad() );
}
//-----------------------------------------------------------------------------
void _CocoaToolkitImpl::startMainApp() {
	[NSApplication sharedApplication];
	[NSApp run];
}
//-----------------------------------------------------------------------------
void _CocoaToolkitImpl::getScreenDimension(Number &outWidth, Number &outHeight) 
{
	NSRect screenRect;
    NSArray *screenArray = [NSScreen screens];
    unsigned screenCount = [screenArray count];
    unsigned index  = 0;
	
    for (; index < screenCount; index++)
    {
        NSScreen *screen = [screenArray objectAtIndex: index];
        screenRect = [screen visibleFrame];
		outWidth+=screenRect.size.width;
		outHeight+=screenRect.size.height;
    }
}
}}} //namespace(s)

#endif //DISCO_USE_COCOA