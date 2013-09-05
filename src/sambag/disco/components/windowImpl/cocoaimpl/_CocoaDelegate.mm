
/*
 *  _CarbonDelegate.mm
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA
#include <sambag/com/Config.h>

#ifdef SAMBAG_64

#include <boost/tuple/tuple.hpp>
#include "_Delegate.h"
#import <objc/runtime.h>
#import <objc/message.h>
#import <iostream>
#import <stdexcept>
#include <iostream>
#include "AutoReleasePool.h"

namespace {
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

} // namespace(s)

namespace arch_delegate {

void setBounds(void *windowRef, NSRect r)
{
}

void disposeCarbonWindow(void *windowRef)
{
}

void * initAsRawWindow(NSWindow *ownerWindow,
                     const NSRect &nsrect)
{
    return NULL;
}



NSWindow * getParentWindowForNesting(void *windowRef)
{
    NSView *view = (NSView*)windowRef;
    return [view window];
}

boost::tuple<float, float> getViewPos (void *windowRef)
{
    NSView *view = (NSView*)windowRef;
    if (!view) {
        return boost::make_tuple(0.f, 0.f);
    }
    NSRect r = getBoundsOnScreen(view);
    return boost::make_tuple((float)r.origin.x, (float)r.origin.y);
}

void attachIssue384Handler(void *parent, NSWindow *nsWindow)
{
}

void closeNestedWindow(NSWindow *window)
{
}

} // namespace(s)


#endif // #SAMBAG_32
#endif // DISCO_USE_COCOA

