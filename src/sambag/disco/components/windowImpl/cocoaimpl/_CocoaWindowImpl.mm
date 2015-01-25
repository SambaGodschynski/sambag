
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
#include <iostream>
#include "AutoReleasePool.h"
#include <boost/tuple/tuple.hpp>
#include <sambag/com/Config.h>
#include "_Delegate.h"


#define SAMBAG_SYNC(x) SAMBAG_BEGIN_SYNCHRONIZED(x)
#define SAMBAG_SYNC_END SAMBAG_END_SYNCHRONIZED

namespace {
	std::string toString(NSString *str) {
		if (!str) {
			return "";
		}
		return std::string([str UTF8String]);
	}
    /*
     * @return bounds of view on screen with origin on top left corner.
     */
    NSRect getBoundsOnScreen(NSView *view) {
        NSRect r = [view frame];
        NSWindow* viewWindow = [view window];
        r.origin = [viewWindow convertBaseToScreen: r.origin];
        r.origin.y = [[[NSScreen screens] objectAtIndex: 0] frame].size.height - r.origin.y - r.size.height;
        return r;
    }
    
    void flipRect(NSRect &r) {
        r.origin.y = [[[NSScreen screens] objectAtIndex: 0] frame].size.height
            - r.origin.y - r.size.height;
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
-(void)setCarbonParentIfNeeded:(void*)theParent;
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
    if (objects) {
        [objects release];
    }
    [super dealloc];
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
-(void)setCarbonParentIfNeeded:(void*)theParent {
    #ifdef SAMBAG_32
        carbonParent = (WindowRef)theParent;
    #endif
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
    NSRect r = windowFrame;
    r.origin.y = [[[NSScreen screens] objectAtIndex: 0] frame].size.height - r.origin.y - r.size.height;
    
    arch_delegate::setBounds(carbonParent, r);

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
    master->____windowBoundsChanged();
}
- (void)windowDidMove:(NSNotification *)notification {
    master->____windowBoundsChanged();
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
    p = [self convertPoint:p fromView:nil];
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
    if (([theEvent modifierFlags] & NSControlKeyMask) > 0) {
        btn = 2;
    }
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
- (void)rightMouseDragged:(NSEvent *)theEvent {
    NSPoint p = [self getMouseLocation: theEvent];
    master->__handleMouseMotionEvent(p.x, p.y);
}
- (void)drawRect:(NSRect)rect {
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
    
}
- (void)setMaster:(Master*) theMaster {
	master = theMaster;
}
-(void)dealloc {
    [super dealloc];
}
- (void)scrollWheel:(NSEvent *)theEvent {
    NSPoint p = [self getMouseLocation: theEvent];
    master->__handleMouseWheelEvent(p.x, p.y, [theEvent deltaY] * -1.);
}
@end


namespace sambag { namespace disco { namespace components {
namespace {
NSWindow * getDiscoWindow(const Master &m) {
	NSWindow *res = (NSWindow*)m.getRawDiscoWindow();
	return res;
}
DiscoView * getDiscoView(const Master &m) {
	DiscoView *res = (DiscoView*)m.getRawDiscoView();
	return res;
}
void deallocSharedPtr(void *ptr) {
    [(NSObject*)ptr release];
}
void deallocCarbonWindowRef(void *ptr) {
    arch_delegate::disposeCarbonWindow(ptr);
}
RawDiscoWindowPtr createDiscoWindowPtr(NSWindow *win) {
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
void _CocoaWindowImpl::invalidateWindow(Number x, Number y, Number w, Number h)
{
    SAMBAG_SYNC( getMutex() )
        AutoReleasePool ap;
        DiscoView *view = getDiscoView(*this);
        if (!view) {
            return; // happens sometimes (maybe while creating)
        }
        [view setNeedsDisplay:YES];
        /*NSRect r = NSMakeRect(x,y,w,h); see #346
        std::cout<<r<<std::endl;
        [view setNeedsDisplayInRect: r];*/
    SAMBAG_SYNC_END
}
//-----------------------------------------------------------------------------
namespace {
DiscoView * _initView(_CocoaWindowImpl *caller, const NSRect &frame) {
	AutoReleasePool ap;
	DiscoWindow *window = (DiscoWindow*)getDiscoWindow(*caller);
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
	AutoReleasePool ap;
    NSWindow *ownerWindow = getDiscoWindow(*this);
    assert(ownerWindow);
    if (!ownerWindow) {
        return;
    }
    
    
    void *wrapperWindow = arch_delegate::initAsRawWindow(ownerWindow, NSMakeRect(x,y,w,h));
   
    if (wrapperWindow) {
        this->carbonWindowRef = createCarbonWindowRefPtr((WindowRef)wrapperWindow);
    }
    __onCreated();
}

//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow(_CocoaWindowImpl *parent, Number x, Number y, Number w, Number h)
{
 	AutoReleasePool ap;
    SAMBAG_SYNC( getMutex() )
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
            NSWindow *pWin = getDiscoWindow(*parent);
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
        [window setAutodisplay: YES];
    
        if (getFlag(sambag::disco::components::WindowFlags::WND_ALWAYS_ON_TOP))
        {
            [window makeKeyAndOrderFront:nil];
            [window setLevel:NSStatusWindowLevel];
        } else {
            [window setLevel:NSFloatingWindowLevel];
        }

    
        if (getFlag(WindowFlags::WND_RAW)) {
            NSRect f = getBoundsOnScreen(view);
            initAsRawWindow(f.origin.x, f.origin.y, f.size.width, f.size.height);
        }

        if (!getFlag(WindowFlags::WND_RAW)) {
            __onCreated();
        }
        [window display];
    SAMBAG_SYNC_END
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openNested(void *parent,
	Number x, Number y, Number w, Number h) 
{
    AutoReleasePool ap;
    #ifdef SAMBAG_32
        NSWindow *window = arch_delegate::getParentWindowForNesting(parent);
        if (!window) {
            throw std::runtime_error("openNested() failed to create window.");
        }
        float xp, yp;
        boost::tie(xp, yp) = arch_delegate::getViewPos(parent);
    
        [window setCanHide: YES];
        [window setIsVisible:YES];
        //[window release];
        int options = 0; //getWindowStyleMask();
        NSRect windowBounds = NSMakeRect(xp, yp, w, h);
        flipRect(windowBounds);
       
        DiscoWindow* pluginWindow  = [[DiscoWindow alloc] initWithContentRect:windowBounds
                                                        styleMask: options
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];
    
        [pluginWindow setCarbonParentIfNeeded: parent];
        
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
    
        arch_delegate::attachIssue384Handler(parent, window);

        __onCreated();
    #else
        NSView *pView = (NSView *) parent;
        NSWindow *window = [pView window];
        this->windowPtr = createDiscoWindowPtr(window);
    	// create view
        DiscoView *view = [
            [DiscoView alloc]
				initWithFrame: NSMakeRect(0,0,w, h)];
        if (!view) {
            throw std::runtime_error("_initView() failed to create view!");
        }
        [view autorelease];
        [pView addSubview:view];
        [window makeFirstResponder: view];
        [view becomeFirstResponder];
        [view setMaster: this];
        this->viewPtr = createDiscoViewPtr(view);
        // tracking area
        NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:[view frame]
									 options:NSTrackingMouseEnteredAndExited | 
									 NSTrackingMouseMoved | 
									 NSTrackingInVisibleRect |
									 NSTrackingActiveAlways |
                                     NSTrackingCursorUpdate
									 owner:view
									 userInfo:nil];
        [trackingArea autorelease];
        [view addTrackingArea:trackingArea];
        __onCreated();
    #endif
}
//-----------------------------------------------------------------------------
void * _CocoaWindowImpl::getWindowRef() const {
#ifdef SAMBAG_32
    AutoReleasePool ap;
	if (carbonWindowRef) {
        return (WindowRef)carbonWindowRef.get();
    }
    NSWindow *win = getDiscoWindow(*this);
	if (!win) {
		return NULL;
	}
    WindowRef ref = (WindowRef)[win windowRef];
    return ref;
#else
    return getDiscoWindow(*this);
#endif // SAMBAG_32
}
//-----------------------------------------------------------------------------
void * _CocoaWindowImpl::getViewRef() const {
    NSWindow * win = getDiscoWindow(*this);
    return [win contentView];
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::closeWindow() {
    boost::shared_ptr<void> hold = __getPtr();
	AutoReleasePool ap;
    NSWindow *window = getDiscoWindow(*this);
	if (!window) {
		return;
	}
    SAMBAG_SYNC( getMutex() )
        if (getFlag(WindowFlags::WND_NESTED)) {
            arch_delegate::closeNestedWindow(window);
            #ifdef SAMBAG_64
                NSView *view = getDiscoView(*this);
                [view setNeedsDisplay:NO];
                [view removeFromSuperview];
                __windowWillCose();
            #endif
        } else {
            [window close];
        }
        this->windowPtr = NULL;
        this->viewPtr = NULL;
    SAMBAG_SYNC_END
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::onClose() {
    SAMBAG_SYNC( getMutex() )
        boost::shared_ptr<void> hold = __getPtr();
        __windowWillCose();
        this->viewPtr = NULL;
        if (getFlag(WindowFlags::EXIT_ON_CLOSE) && !getFlag(WindowFlags::WND_NESTED))
        {
            _CocoaToolkitImpl::quit();
        }
    SAMBAG_SYNC_END
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::setBounds(Number x, Number y, Number w, Number h) {
	AutoReleasePool ap;
    NSWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
    if (!window || !view) {
        return;
    }
    Number sw=0, sh=0;
    _CocoaToolkitImpl::getScreenDimension(sw, sh);
    NSRect frame = NSMakeRect(x, sh - y - h, w, h);
    
    bool isNestedAndCocoa64 = getFlag(WindowFlags::WND_NESTED);
    #ifdef SAMBAG_32
        isNestedAndCocoa64 = false;
    #endif
    
    if (isNestedAndCocoa64) {
        if (!view) {
            return;
        }
        [view setFrameSize: frame.size];
        __boundsChanged(frame.origin.x,
				  frame.origin.y, 
				  frame.size.width, 
				  frame.size.height);
        return;
    }
    
    if (window) {
        NSRect winFrame = [window frameRectForContentRect:frame];
        [window setFrame:winFrame display:YES animate:NO];
        [window setContentSize: frame.size];
    }
	// view
    if (view) {
        frame = [window contentRectForFrameRect:[window frame]];
        [view setFrameSize: frame.size];

    }
   if (carbonWindowRef && view) {
        NSRect f = getBoundsOnScreen(view);
        WindowRef win = (WindowRef)carbonWindowRef.get();
        arch_delegate::setBounds(win, f);
    }
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::getBounds(Number &x, Number &y, Number &w, Number &h) const
{
	AutoReleasePool ap;
    NSWindow *window = getDiscoWindow(*this);
    DiscoView *view = getDiscoView(*this);
    NSRect frame = [view convertRect:[view frame] toView:[window contentView]];
    frame.origin = [window convertBaseToScreen: frame.origin];
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
	AutoReleasePool ap;
	NSWindow *window = getDiscoWindow(*this);
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
    
   if (carbonWindowRef && view) {
        NSRect f = getBoundsOnScreen(view);
        WindowRef win = (WindowRef)carbonWindowRef.get();
        arch_delegate::setBounds(win, f);
   }
}
//-----------------------------------------------------------------------------
std::string _CocoaWindowImpl::getTitle() const {
	AutoReleasePool ap;
	NSWindow *window = getDiscoWindow(*this);
	if (!window) {
		return "";
	}
	return toString( [window title] );
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::setTitle(const std::string &str) {
	AutoReleasePool ap;
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
void _CocoaToolkitImpl::loadMainApp() {
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


//-----------------------------------------------------------------------------
void * __getHandlerForVstPlugins_(void *ptr) {
    #ifdef SAMBAG_32
        return ptr;
    #else
    if (!ptr) {
        return NULL;
    }
    NSWindow *win = (NSWindow*)ptr;
    return [win contentView];
    #endif
}


#endif //DISCO_USE_COCOA