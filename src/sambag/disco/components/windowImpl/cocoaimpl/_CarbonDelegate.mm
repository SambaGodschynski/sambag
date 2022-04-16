
/*
 *  _CarbonDelegate.mm
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#include <sambag/com/Config.h>

#ifdef SAMBAG_32



#include <boost/tuple/tuple.hpp>
#include "_Delegate.h"
#import <objc/runtime.h>
#import <objc/message.h>
#import <iostream>
#import <stdexcept>
#include <iostream>
#include "AutoReleasePool.h"
#import <Carbon/Carbon.h>

namespace {
pascal OSStatus
issue384WorkaroundHandler(EventHandlerCallRef, EventRef e, void* user)
{
    NSWindow* hostWindow = (NSWindow*) user;

    switch (GetEventKind (e))
    {
        case kEventWindowInit:    [hostWindow display]; break;
        case kEventWindowShown:   [hostWindow orderFront: nil]; break;
        case kEventWindowHidden:  [hostWindow orderOut: nil]; break;
    }

    return eventNotHandledErr;
}
HIViewRef _getViewRef(WindowRef windowRef)
{
    HIViewRef result = NULL;
    WindowAttributes attributes;
    GetWindowAttributes ((WindowRef) windowRef, &attributes);
    if ((attributes & kWindowCompositingAttribute) != 0)
    {
        HIViewRef root = HIViewGetRoot (windowRef);
        HIViewFindByID (root, kHIViewWindowContentID, &result);

        if (result == 0)
            result = root;
    }
    else
    {
        GetRootControl (windowRef, (ControlRef*) &result);

        if (result == 0)
            CreateRootControl (windowRef, (ControlRef*) &result);
    }
    return result;
}
} // namespace(s)

namespace arch_delegate {

void setBounds(void *windowRef, NSRect r)
{
    WindowRef carbonParent = (WindowRef)(windowRef);
    Rect wr;
    wr.left   = (short) r.origin.x;
    wr.top    = (short) r.origin.y;
    wr.right  = (short) r.origin.x + r.size.width;
    wr.bottom = (short) r.origin.y + r.size.height;
    SetWindowBounds (carbonParent, kWindowContentRgn, &wr);
    ShowWindow (carbonParent);
}

void disposeCarbonWindow(void *windowRef)
{
    DisposeWindow ((WindowRef)windowRef);
}

void * initAsRawWindow(NSWindow *ownerWindow,
                     const NSRect &nsrect)
{
    Rect r;
    r.left   = (short) nsrect.origin.x;
    r.top    = (short) nsrect.origin.y;
    r.right  = (short) nsrect.origin.x+nsrect.size.width;
    r.bottom = (short) nsrect.origin.y+nsrect.size.height;
    
    WindowRef wrapperWindow;
	
    CreateNewWindow (kDocumentWindowClass,
                        (WindowAttributes) (kWindowStandardHandlerAttribute | kWindowCompositingAttribute
                        | kWindowNoShadowAttribute | kWindowNoTitleBarAttribute),
                         &r, &wrapperWindow);
    
    assert(wrapperWindow);
    if (wrapperWindow == 0)
        return NULL;
    
    NSWindow *carbonWindow = [[NSWindow alloc] initWithWindowRef: wrapperWindow];
        
    [ownerWindow addChildWindow: carbonWindow
                        ordered: NSWindowAbove];
    
    return wrapperWindow;
}

void attachIssue384Handler(void *parent, NSWindow *nsWindow)
{
    WindowRef hostWindowRef = (WindowRef)parent;
    const EventTypeSpec eventsToCatch[] =
    {
        { kEventClassWindow, kEventWindowInit },
        { kEventClassWindow, kEventWindowShown },
        { kEventClassWindow, kEventWindowHidden }
    };

    EventHandlerRef ref;
    InstallWindowEventHandler (hostWindowRef,
                               NewEventHandlerUPP (issue384WorkaroundHandler),
                               GetEventTypeCount (eventsToCatch), eventsToCatch,
                               (void*) nsWindow, &ref);
}


NSWindow * getParentWindowForNesting(void *windowRef)
{
    WindowRef parent = (WindowRef)windowRef;
    HIViewRef parentView = _getViewRef(parent);
    if (!parentView) {
        return NULL;
    }
    NSWindow *window = [[NSWindow alloc] initWithWindowRef:parent];
    return window;
}

boost::tuple<float, float> getViewPos (void *windowRef)
{
    HIViewRef view = _getViewRef((WindowRef)windowRef);
    if (!view) {
        return boost::make_tuple(0.f,0.f);
    }
    HIRect r;
    HIViewGetFrame (view, &r);
    HIViewRef root;
    HIViewFindByID (HIViewGetRoot (HIViewGetWindow (view)), kHIViewWindowContentID, &root);
    HIViewConvertRect (&r, HIViewGetSuperview (view), root);

    Rect windowPos;
    GetWindowBounds (HIViewGetWindow (view), kWindowContentRgn, &windowPos);

    return boost::make_tuple(windowPos.left + r.origin.x,
        windowPos.top + r.origin.y);
}

void closeNestedWindow(NSWindow *window)
{
    if (!window) {
        return;
    }
    NSWindow * parent = [window parentWindow];
    if (parent) {
        [parent removeChildWindow: window];
    }
    [window close];
}

} // namespace(s)


#endif // #SAMBAG_32
#endif // DISCO_USE_COCOA

