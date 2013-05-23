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
{
    NSMutableArray *objects;
}
-(void)initObjects;
-(void)dealloc;
-(void)addObject:(NSObject*)obj;
-(void)removeObject:(NSObject*)obj;
@end
@implementation DiscoWindow
-(void)initObjects {
    if (objects) {
        return;
    }
    objects = [[NSMutableArray alloc] init];
}
-(void)dealloc {
    std::cout<<"-- DEALLOC WINDOW"<<std::endl;
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
    std::cout<<"-- DEALLOC DELEGATE"<<std::endl;
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
		 rect.size.height
	);
}
- (void)setMaster:(Master*) theMaster {
	master = theMaster;
}
-(void)dealloc {
    std::cout<<"-- DEALLOC VIEW"<<std::endl;
    [super dealloc];
}
@end


namespace sambag { namespace disco { namespace components {
namespace {
DiscoWindow * getDiscoWindow(const Master &m) {
	DiscoWindow *res = (DiscoWindow*)m.getRawDiscoWindow().get();
	//assert(res);
	return res;
}
DiscoView * getDiscoView(const Master &m) {
	DiscoView *res = (DiscoView*)m.getRawDiscoView().get();
	//assert(res);
	return res;
}
void deallocSharedPtr(void *ptr) {
    [(NSObject*)ptr release];
}
void deallocCarbonWindowRef(void *ptr) {
     
}
RawDiscoWindowPtr createDiscoWindowPtr(DiscoWindow *win) {
    [win retain];
    return RawDiscoWindowPtr(win, &deallocSharedPtr);
}
RawDiscoViewPtr createDiscoViewPtr(DiscoView *view) {
    [view retain];
    return RawDiscoViewPtr(view, &deallocSharedPtr);
}
CarbonWindowRefPtr createCarbonWindowRefPtr(WindowRef ref) {
    return CarbonWindowRefPtr(ref, &deallocCarbonWindowRef);
}
NSRect getBoundsOnScreen(NSView *view) {
    NSRect r = [view frame];
    NSWindow* viewWindow = [view window];
    //r = [[view superview] convertRect: r toView: nil];
    r.origin = [viewWindow convertBaseToScreen: r.origin];
    r.origin.y = [[[NSScreen screens] objectAtIndex: 0] frame].size.height - r.origin.y - r.size.height;
    return r;
}
} // namepsace(s)
//=============================================================================
// @class _CocoaWindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
_CocoaWindowImpl::_CocoaWindowImpl()
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
		options &= NSClosableWindowMask;
		options &= ~NSMiniaturizableWindowMask;
	}
	return options;
}
//-----------------------------------------------------------------------------
namespace {
void recursiveHIViewRepaint (HIViewRef view)
{
    HIViewSetNeedsDisplay (view, true);
    HIViewRef child = HIViewGetFirstSubview (view);
        
    while (child != 0)
    {
        recursiveHIViewRepaint (child);
        child = HIViewGetNextView (child);
    }
}
void __windowCheck(WindowRef wrapperWindow,
                   int c)
{
    
    recursiveHIViewRepaint (HIViewGetRoot (wrapperWindow));
    if (c<100) {
        sambag::disco::components::getWindowToolkit()->invokeLater(
            boost::bind(&__windowCheck, wrapperWindow, c+1),
            10
        );
    }

}
void ____(WindowRef window) {
    HIViewRef root_control;
    HIViewRef viewRef;
    GetRootControl (window, &root_control);
    viewRef = HIViewGetFirstSubview(root_control);
    if (!viewRef) {
        std::cout<<"no view"<<std::endl;
        return;
    }
    HIRect bnd;
    HIViewGetBounds(viewRef, &bnd);
    std::cout<<"view :"<<std::hex<<viewRef<<std::dec<<std::endl;
    std::cout<<"  --bounds:"<<bnd.origin.x<<", "<<bnd.origin.y<<", "<<bnd.size.width<<", "<<bnd.size.height<<std::endl;
    std::cout<<"  --isVisible:"<<(HIViewIsVisible(viewRef)==0 ? "no":"yes")<<std::endl;
    std::cout<<"  --isEnabled:"<<(HIViewIsEnabled(viewRef, NULL)==0 ? "no":"yes")<<std::endl;
    std::cout<<"  --isDrawingEnabled:"<<(HIViewIsDrawingEnabled(viewRef)==0 ? "no":"yes")<<std::endl;
    std::cout<<"  --HIViewIsCompositingEnabled:"<<(HIViewIsCompositingEnabled(viewRef)==0 ? "no":"yes")<<std::endl;
    
    sambag::disco::components::getWindowToolkit()->invokeLater(
        boost::bind(&____, window),
        5000
    );
    
    HIViewSetVisible(viewRef, true);
    //HIViewSetBoundsOrigin(viewRef, bnd.origin.x, bnd.origin.y);
    HIViewSetNeedsDisplay(viewRef, true);
}
} // namespace(s)
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::initAsRawWindow(Number x, Number y, Number w, Number h)
{
    DiscoWindow *ownerWindow = getDiscoWindow(*this);
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
    
    __onCreated();
  
    // Check for the plugin creating its own floating window, and if there is one,
    // we need to reparent it to make it visible..
   /* NSWindow* floatingChildWindow = [[carbonWindow childWindows] objectAtIndex: 0];
    
    if (floatingChildWindow != nil) {
        std::cout<<"has own floating window"<<std::endl;
        [ownerWindow addChildWindow: floatingChildWindow
                                 ordered: NSWindowAbove];
    }*/
}

//-----------------------------------------------------------------------------
void _CocoaWindowImpl::openWindow(_CocoaWindowImpl *parent, Number x, Number y, Number w, Number h) 
{
    Number sw=0, sh=0;
    _CocoaToolkitImpl::getScreenDimension(sw, sh);
	NSRect frame = NSMakeRect(x,sh - y - h,w,h);
	int options = getWindowStyleMask();
	DiscoWindow* window  = [[DiscoWindow alloc] initWithContentRect:frame
						  styleMask: options
						  backing:NSBackingStoreBuffered
						  defer:YES];
    [window setBackgroundColor: [NSColor blueColor]];
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
    NSWindow *window = [[NSWindow alloc] initWithWindowRef:parent];
	if (!window) {
		throw std::runtime_error("openNested() failed to create window.");
	}
    //[window retain];
    [window setCanHide: YES];
    [window setReleasedWhenClosed: YES];
    [window setIsVisible:YES];
    int options = 0; //getWindowStyleMask();
    NSRect windowBounds = [window contentRectForFrameRect:[window frame]];
    windowBounds.size.width = w;
    windowBounds.size.height = h;
   
	DiscoWindow* pluginWindow  = [[DiscoWindow alloc] initWithContentRect:windowBounds
                                                        styleMask: options
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];

	
    [pluginWindow setAcceptsMouseMovedEvents:YES];
    this->windowPtr = createDiscoWindowPtr(pluginWindow);
    [window addChildWindow:pluginWindow ordered:NSWindowAbove];
    [window orderFront: nil];
    
    attachWindowHidingHooks(parent, window);

    NSRect frame = NSMakeRect(0,0,w,h);
	DiscoView * view = _initView(this, frame);
    if (!view) {
        return;
    }
	// assign raw pointer
	this->viewPtr = createDiscoViewPtr(view);
    
    __onCreated();
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
	[window close];
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
    if (carbonWindowRef && view) {
        NSRect f = getBoundsOnScreen(view);
        Rect wr;
        wr.left   = (short) f.origin.x;
        wr.top    = (short) f.origin.y;
        wr.right  = (short) f.origin.x + f.size.width;
        wr.bottom = (short) f.origin.y + f.size.height;
        WindowRef win = (WindowRef)carbonWindowRef.get();
        SetWindowBounds (win, kWindowContentRgn, &wr);
        ShowWindow (win);
     }
}
//-----------------------------------------------------------------------------
void _CocoaWindowImpl::getBounds(Number &x, Number &y, Number &w, Number &h) {
    DiscoWindow *window = getDiscoWindow(*this);
	DiscoView *view = getDiscoView(*this);
	NSRect frame = [window contentRectForFrameRect:[window frame]];
	[view setFrameSize: frame.size];
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
    
    
    if (carbonWindowRef && view) {
        NSRect f = getBoundsOnScreen(view);
        Rect wr;
        wr.left   = (short) f.origin.x;
        wr.top    = (short) f.origin.y;
        wr.right  = (short) f.origin.x + f.size.width;
        wr.bottom = (short) f.origin.y + f.size.height;
        WindowRef win = (WindowRef)carbonWindowRef.get();
        SetWindowBounds (win, kWindowContentRgn, &wr);
        ShowWindow (win);
    }
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
void _CocoaWindowImpl::onClose() {
    __windowWillCose();
    this->windowPtr.reset();
    this->viewPtr.reset();
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