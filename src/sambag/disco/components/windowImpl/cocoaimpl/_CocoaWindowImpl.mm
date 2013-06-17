
/*
 * _CocoaWindowImpl.mm
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#include <sambag/disco/components/WindowToolkit.hpp>
#import "_CocoaWindowImpl.h"
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>
#import <objc/message.h>
#import <iostream>
#import <stdexcept>
#import <Carbon/Carbon.h>
#include "AutoReleasePool.h"


#define NOP(x)

namespace {
	std::string toString(NSString *str) {
		if (!str) {
			return "";
		}
		return std::string([str UTF8String]);
	}
    NSRect getBoundsOnScreen(NSView *view) {
        NSRect r = [view frame];
        NSWindow* viewWindow = [view window];
        r.origin = [viewWindow convertBaseToScreen: r.origin];
        r.origin.y = [[[NSScreen screens] objectAtIndex: 0] frame].size.height - r.origin.y - r.size.height;
        return r;
    }

} // namespace(s)

typedef sambag::disco::components::_CocoaWindowImpl Master;
//=========================================================================
@interface DiscoWindow : NSWindow
{
    NSMutableArray *objects;
    WindowRef carbonParent;
    Master *master;
}
-(void)initObjects;
-(void)dealloc;
-(void)addObject:(NSObject*)obj;
-(void)removeObject:(NSObject*)obj;
-(void)setMaster:(Master*) theMaster;
-(WindowRef)carbonParent;
-(void)setCarbonParent:(WindowRef)theParent;
-(void)setFrame:(NSRect)windowFrame
        display:(BOOL)displayViews
        animate:(BOOL)performAnimation;
@end
@implementation DiscoWindow
-(void)initObjects {
    if (objects) {
        return;
    }
    objects = [[NSMutableArray alloc] init];
}
-(void)dealloc {
    [super dealloc];
    if (objects) {
        [objects release];
    }
}
-(void)addObject:(NSObject*)obj {
    [self initObjects];
    [objects addObject: obj];

}
-(void)removeObject:(NSObject*)obj {
    [self initObjects];
    [objects removeObject: obj];
}
-(WindowRef)carbonParent {
    return carbonParent;
}
-(void)setCarbonParent:(WindowRef)theParent {
    carbonParent = theParent;
}
-(void)setMaster:(Master*) theMaster{
	master = theMaster;
}
-(void)setFrame:(NSRect)windowFrame
        display:(BOOL)displayViews
        animate:(BOOL)performAnimation
{
    [super setFrame:windowFrame
           display:displayViews
            animate:performAnimation];
    
    if (!carbonParent) {
        return;
    }
    NSRect f = getBoundsOnScreen([self contentView]);
    Rect wr;
    wr.left   = (short) f.origin.x;
    wr.top    = (short) f.origin.y;
    wr.right  = (short) f.origin.x + f.size.width;
    wr.bottom = (short) f.origin.y + f.size.height;
    NOP(SetWindowBounds (carbonParent, kWindowContentRgn, &wr);
    ShowWindow (carbonParent);)
}
@end

//=========================================================================
@interface DiscoWindowDelegate : NSObject<NSWindowDelegate>
{
	Master *master;
}
- (void)windowDidResize:(NSNotification *)notification;
- (void)windowDidMove:(NSNotification *)notification;
- (void)setMaster:(Master*) theMaster;
- (void)dealloc;
- (void)windowWillClose:(NSNotification *)notification;
@end

@implementation DiscoWindowDelegate
- (void)windowDidResize:(NSNotification *)notification {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        master->____windowBoundsChanged();
    SAMBAG_END_SYNCHRONIZED
}
- (void)windowDidMove:(NSNotification *)notification {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        master->____windowBoundsChanged();
    SAMBAG_END_SYNCHRONIZED
}
- (void)setMaster:(Master*) theMaster{
	master = theMaster;
}
-(void)dealloc {
    [super dealloc];
}
- (void)windowWillClose:(NSNotification *)notification {
    master->onClose();
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
- (void)scrollWheel:(NSEvent *)theEvent;
- (void)rightMouseDragged:(NSEvent *)theEvent;
- (BOOL)acceptsFirstResponder;
- (void)drawRect:(NSRect)rect;
- (void)setMaster:(Master*) theMaster;
- (NSPoint)flipPoint:(NSPoint)p;
- (NSRect)flipRect:(NSRect)r;
- (NSPoint)getMouseLocation:(NSEvent *)theEvent;
- (int)getMouseBtn:(NSEvent*) ev;
- (void)dealloc;
@end


@implementation DiscoView

- (NSRect)flipRect:(NSRect)r {
	NSRect frame = [self frame];
	r.origin.y = frame.size.height - r.origin.y;
	return r;
}

- (NSPoint)flipPoint:(NSPoint)p {
	NSRect frame = [self bounds];
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
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        int btn = [self getMouseBtn: theEvent];
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseButtonPressEvent(p.x, p.y, btn);
    SAMBAG_END_SYNCHRONIZED
}
- (void)mouseUp:(NSEvent *)theEvent {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        int btn = [self getMouseBtn: theEvent];
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseButtonReleaseEvent(p.x, p.y, btn);
    SAMBAG_END_SYNCHRONIZED
}
- (void)rightMouseDown:(NSEvent *)theEvent{
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        int btn = [self getMouseBtn: theEvent];
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseButtonPressEvent(p.x, p.y, btn);
    SAMBAG_END_SYNCHRONIZED
}
- (void)rightMouseUp:(NSEvent *)theEvent{
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        int btn = [self getMouseBtn: theEvent];
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseButtonReleaseEvent(p.x, p.y, btn);
    SAMBAG_END_SYNCHRONIZED
}
- (void)mouseMoved:(NSEvent *)theEvent {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseMotionEvent(p.x, p.y);
    SAMBAG_END_SYNCHRONIZED
}
- (void)mouseDragged:(NSEvent *)theEvent{
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseMotionEvent(p.x, p.y);
    SAMBAG_END_SYNCHRONIZED
}
- (void)rightMouseDragged:(NSEvent *)theEvent {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseMotionEvent(p.x, p.y);
    SAMBAG_END_SYNCHRONIZED
}
- (void)drawRect:(NSRect)rect {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        if (! [[self window] isVisible]) {
            return;
        }
        void *gc = [[[self window] graphicsContext] graphicsPort];
        master->__processDraw(
                              (CGContextRef)gc,
                              rect.origin.x, 
                              rect.origin.y, 
                              rect.size.width, 
                              rect.size.height);
    SAMBAG_END_SYNCHRONIZED
}
- (void)setMaster:(Master*) theMaster {
	master = theMaster;
}
-(void)dealloc {
    [super dealloc];
}
- (void)scrollWheel:(NSEvent *)theEvent {
    SAMBAG_BEGIN_SYNCHRONIZED( master->getMutex() )
        NSPoint p = [self getMouseLocation: theEvent];
        master->__handleMouseWheelEvent(p.x, p.y, [theEvent deltaY] * -1.);
    SAMBAG_END_SYNCHRONIZED
    
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
void deallocSharedPtr(void *ptr) {
    [(NSObject*)ptr release];
}
void deallocCarbonWindowRef(void *ptr) {
    //DisposeWindow ((WindowRef)ptr);
}
RawDiscoWindowPtr createDiscoWindowPtr(DiscoWindow *win) {
    return win;
}
RawDiscoViewPtr createDiscoViewPtr(DiscoView *view) {
    return view;
}
CarbonWindowRefPtr createCarbonWindowRefPtr(WindowRef ref) {
    return CarbonWindowRefPtr(ref, &deallocCarbonWindowRef);
}
} // namepsace(s)
//=============================================================================
// @class _CocoaWindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
_CocoaWindowImpl::_CocoaWindowImpl() :
windowPtr(NULL),
viewPtr(NULL)
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
	[window setContentView:view];
	[window setMaster: caller];
    [window makeFirstResponder: view];
	[view becomeFirstResponder];
	[view setMaster: caller];
    [view release];
	
	// tracking area
	NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:[view frame]
									 options:NSTrackingMouseEnteredAndExited | 
									 NSTrackingMouseMoved | 
									 NSTrackingInVisibleRect |
									 NSTrackingActiveAlways
									 owner:view
									 userInfo:nil];
	
	[view addTrackingArea:trackingArea];
    [trackingArea release];
	
	// set delegate
	DiscoWindowDelegate *delegate = [[DiscoWindowDelegate alloc] init];
	[delegate setMaster: caller];
	[window setDelegate:delegate];
    [window addObject:delegate];
    [delegate release];
	
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
		options &= ~NSClosableWindowMask;
		options &= ~NSMiniaturizableWindowMask;
	}
	return options;
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::initAsRawWindow(Number x, Number y, Number w, Number h)
{
    NOP(DiscoWindow *ownerWindow = getDiscoWindow(*this);
    assert(ownerWindow);
    if (!ownerWindow) {
        return;
    }
    
    Rect r;
    r.left   = (short) x;
    r.top    = (short) y;
    r.right  = (short) x+w;
    r.bottom = (short) y+h;
    
    WindowRef wrapperWindow;
	
    CreateNewWindow (kDocumentWindowClass,
                        (WindowAttributes) (kWindowStandardHandlerAttribute | kWindowCompositingAttribute
                        | kWindowNoShadowAttribute | kWindowNoTitleBarAttribute),
                         &r, &wrapperWindow);
    
    assert(wrapperWindow);
    if (wrapperWindow == 0)
        return;
    
    this->carbonWindowRef = createCarbonWindowRefPtr(wrapperWindow);
    
    NSWindow *carbonWindow = [[NSWindow alloc] initWithWindowRef: wrapperWindow];
        
    [ownerWindow addChildWindow: carbonWindow
                        ordered: NSWindowAbove];
    
    __onCreated();)
}

//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow(_CocoaWindowImpl *parent, Number x, Number y, Number w, Number h)
{
    SAMBAG_BEGIN_SYNCHRONIZED( getMutex() )
        Number sw=0, sh=0;
        _CocoaToolkitImpl::getScreenDimension(sw, sh);
        NSRect frame = NSMakeRect(x,sh - y - h,w,h);
        int options = getWindowStyleMask();
        DiscoWindow* window  = [[DiscoWindow alloc] initWithContentRect:frame
                                                    styleMask: options
                                                    backing:NSBackingStoreBuffered
                                                    defer:YES];
        this->windowPtr = createDiscoWindowPtr(window);
        // set parent
        if (parent) {
            DiscoWindow *pWin = getDiscoWindow(*parent);
            if (pWin) {
                //[window setParentWindow:pWin];
            }
        }
        DiscoView* view = NULL;
        view = _initView(this, frame);
        assert(view);
        if (!view) {
            return;
        }
    
        this->viewPtr = createDiscoViewPtr(view);
    
        [window makeKeyAndOrderFront:nil];
        [window setAcceptsMouseMovedEvents:YES];
        [window setReleasedWhenClosed: YES];
        [window setLevel: NSFloatingWindowLevel];
        [window setAutodisplay: YES];
    
        if (getFlag(WindowFlags::WND_RAW)) {
            NSRect f = getBoundsOnScreen(view);
            initAsRawWindow(f.origin.x, f.origin.y, f.size.width, f.size.height);
        }

        if (!getFlag(WindowFlags::WND_RAW)) {
            __onCreated();
        }
        [window display];
    SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openNested(WindowRef parent,
	Number x, Number y, Number w, Number h) 
{
    SAMBAG_BEGIN_SYNCHRONIZED( getMutex() )
        NSWindow *window = [[NSWindow alloc] initWithWindowRef:parent];
        if (!window) {
            throw std::runtime_error("openNested() failed to create window.");
        }
        [window setCanHide: YES];
        [window setIsVisible:YES];
        [window release];
        int options = 0; //getWindowStyleMask();
        NSRect windowBounds = [window contentRectForFrameRect:[window frame]];
        windowBounds.size.width = w;
        windowBounds.size.height = h;
   
        DiscoWindow* pluginWindow  = [[DiscoWindow alloc] initWithContentRect:windowBounds
                                                        styleMask: options
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];
        [pluginWindow setCarbonParent: parent];
        this->windowPtr = createDiscoWindowPtr(pluginWindow);

        // show window
        [pluginWindow setAcceptsMouseMovedEvents:YES];
        [pluginWindow setReleasedWhenClosed:YES];
        [window addChildWindow:pluginWindow ordered:NSWindowAbove];
        [window orderFront: nil];
        NSRect frame = NSMakeRect(0,0,w,h);
        DiscoView * view = _initView(this, frame);
        if (!view) {
            return;
        }
        // assign raw pointer
        this->viewPtr = createDiscoViewPtr(view);
        __onCreated();
    SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
WindowRef _CocoaWindowImpl::getWindowRef() const {
	if (carbonWindowRef) {
        return (WindowRef)carbonWindowRef.get();
    }
    NSWindow *win = getDiscoWindow(*this);
	if (!win) {
		return NULL;
	}
    WindowRef ref = (WindowRef)[win windowRef];
    return ref;
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::closeWindow() {
	DiscoWindow *window = getDiscoWindow(*this);
	if (!window) {
		return;
	}
    SAMBAG_BEGIN_SYNCHRONIZED( getMutex() )
        if (getFlag(WindowFlags::WND_NESTED)) {
            NSWindow * parent = [window parentWindow];
            if (parent) {
                [parent removeChildWindow: window];
            }
        }
    
        [window close];
        this->carbonWindowRef.reset();
        this->windowPtr = NULL;
        this->viewPtr = NULL;
    SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::onClose() {
    __windowWillCose();
    if (getFlag(WindowFlags::EXIT_ON_CLOSE) && !getFlag(WindowFlags::WND_NESTED))
    {
        _CocoaToolkitImpl::quit();
    }
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::setBounds(Number x, Number y, Number w, Number h) {
    
    DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
    Number sw=0, sh=0;
    _CocoaToolkitImpl::getScreenDimension(sw, sh);
    NSRect frame = NSMakeRect(x, sh - y - h, w, h);
    if (window) {
        [window setFrame:frame display:YES animate:NO];
        [window setContentSize: frame.size];        
    }
	// view
    if (view) {
      	frame = [window contentRectForFrameRect:[window frame]];
        [view setFrameSize: frame.size];

    }
    NOP(if (carbonWindowRef && view) {
        NSRect f = getBoundsOnScreen(view);
        Rect wr;
        wr.left   = (short) f.origin.x;
        wr.top    = (short) f.origin.y;
        wr.right  = (short) f.origin.x + f.size.width;
        wr.bottom = (short) f.origin.y + f.size.height;
        WindowRef win = (WindowRef)carbonWindowRef.get();
        SetWindowBounds (win, kWindowContentRgn, &wr);
        ShowWindow (win);
     })
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::getBounds(Number &x, Number &y, Number &w, Number &h) const
{
    DiscoWindow *window = getDiscoWindow(*this);
    NSRect frame = [window contentRectForFrameRect:[window frame]];
    Number sw=0, sh=0;
    _CocoaToolkitImpl::getScreenDimension(sw, sh);
    frame.origin.y = sh - frame.origin.y - frame.size.height;
	x = frame.origin.x;
    y = frame.origin.y;
    w = frame.size.width;
    h = frame.size.height;

}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::____windowBoundsChanged() {
	DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
	NSRect frame = [window contentRectForFrameRect:[window frame]];
	[view setFrameSize: frame.size];
    Number sw=0, sh=0;
    _CocoaToolkitImpl::getScreenDimension(sw, sh);
    frame.origin.y = sh - frame.origin.y - frame.size.height;
	__boundsChanged(frame.origin.x,
				  frame.origin.y, 
				  frame.size.width, 
				  frame.size.height);
    
    NOP(if (carbonWindowRef && view) {
        NSRect f = getBoundsOnScreen(view);
        Rect wr;
        wr.left   = (short) f.origin.x;
        wr.top    = (short) f.origin.y;
        wr.right  = (short) f.origin.x + f.size.width;
        wr.bottom = (short) f.origin.y + f.size.height;
        WindowRef win = (WindowRef)carbonWindowRef.get();
        SetWindowBounds (win, kWindowContentRgn, &wr);
    })
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
//-----------------------------------------------------------------------------
_CocoaWindowImpl::~_CocoaWindowImpl() {
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
    AutoReleasePool ap;
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
	
    NSScreen *screen = [screenArray objectAtIndex: index];
    screenRect = [screen visibleFrame];
    outWidth=screenRect.size.width;
    outHeight=screenRect.size.height;
    index++;
    for (; index < screenCount; index++)
    {
        screen = [screenArray objectAtIndex: index];
		outWidth+=screenRect.size.width;
    }
}
//-----------------------------------------------------------------------------
void _CocoaToolkitImpl::quit() {
    //[NSApp terminate:nil];
    [NSApp stop:nil];
    // dummy event:
    // http://www.cocoabuilder.com/archive/cocoa/219842-nsapp-stop.html
    AutoReleasePool ap;
    NSEvent* event = [NSEvent otherEventWithType: NSApplicationDefined
                                        location: NSMakePoint(0,0)
                                   modifierFlags: 0
                                       timestamp: 0.0
                                    windowNumber: 0
                                         context: nil
                                         subtype: 0
                                           data1: 0
                                           data2: 0];
    [NSApp postEvent: event atStart: true];
}
}}} //namespace(s)

#endif //DISCO_USE_COCOA