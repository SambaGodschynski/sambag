/*
 *  _Delegate.h
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */

#ifndef DISCO_DELEGATE
#define DISCO_DELEGATE

#include <boost/tuple/tuple.hpp>
#import <Cocoa/Cocoa.h>


/**
 * Since carbon isn't supported by Cocoa 64-bit anymore, we need to
 * delegate some functions.
 */
namespace arch_delegate {
    void setBounds(void *windowRef, NSRect r);
    void disposeCarbonWindow(void *windowRef);
    void * initAsRawWindow(NSWindow *ownerWindow,
                         const NSRect &r);
    void attachIssue384Handler(void *parent, NSWindow *window);
    NSWindow * getParentWindowForNesting(void *parent);
    boost::tuple<float, float> getViewPos (void *parent);

} // namespace(s)



#endif //‚DISCO_DELEGATE