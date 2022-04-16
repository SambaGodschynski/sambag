/*
 * _CocoaTimer.mm
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA

#import "_CocoaTimer.h"
#import <Cocoa/Cocoa.h>
#include <sambag/disco/components/Timer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <sambag/com/Thread.hpp>
#include "AutoReleasePool.h"

using sambag::disco::components::_CocoaTimer;
typedef _CocoaTimer::TimerPtr TimerPtr;
namespace {
    sambag::com::RecursiveMutex mapLock;
    typedef boost::unordered_map<TimerPtr, NSTimer*> TimerMap;
    TimerMap timers;
    NSTimer * getOsTimer(TimerPtr tm) {
        SAMBAG_BEGIN_SYNCHRONIZED(mapLock)
            TimerMap::iterator it = timers.find(tm);
            if (it==timers.end()) {
                return NULL;
            }
            return it->second;
        SAMBAG_END_SYNCHRONIZED
    }
    void stopTimerImpl(TimerPtr _tm, NSTimer *osTimer) {
        [osTimer invalidate];
        SAMBAG_BEGIN_SYNCHRONIZED(mapLock)
            timers.erase(_tm);
        SAMBAG_END_SYNCHRONIZED
    }
    void registerTimer(TimerPtr _tm, NSTimer *osTimer) {
        SAMBAG_BEGIN_SYNCHRONIZED(mapLock)
            timers.insert( TimerMap::value_type(_tm, osTimer) );
        SAMBAG_END_SYNCHRONIZED
    }
}
//=========================================================================
@interface _Timer : NSObject 
{
    TimerPtr _tm;
}
-(void)onTick:(NSTimer*)osTimer;
-(id)initWithTimerPtr:(TimerPtr)ptr;
-(void)dealloc;
@end

@implementation _Timer
-(id)initWithTimerPtr:(TimerPtr)ptr {
    [super init];
    _tm = ptr;
    return self;
}
-(void)onTick:(NSTimer*)osTimer {
    AutoReleasePool ap;
    _tm->timerExpired();
    int &numCalled = _tm->__getNumCalled_();
    ++numCalled;
    if (_tm->getNumRepetitions() != -1 &&
		numCalled > _tm->getNumRepetitions())
    {
        stopTimerImpl(_tm, osTimer);
        return;
    }
    NSTimeInterval tmDelay = _tm->getDelay() / 1000.;
    NSTimeInterval osDelay = [osTimer timeInterval];
    if (tmDelay != osDelay) {
        [osTimer setFireDate:[NSDate dateWithTimeIntervalSinceNow: tmDelay]];
    }
}
-(void)dealloc {
    [super dealloc];
}
@end

namespace sambag { namespace disco { namespace components {
//=============================================================================
// class _CocoaTimer 
//=============================================================================
//-----------------------------------------------------------------------------
void _CocoaTimer::startTimer(TimerPtr tm)
{
    if (!tm) {
        return;
    }
    AutoReleasePool ap;
    NSTimeInterval i = tm->getInitialDelay() / 1000.;
    
    NSTimer *t = getOsTimer(tm);
    if (t) { // timer already running
        [t setFireDate:[NSDate dateWithTimeIntervalSinceNow: i]];
        return;
    }
    
    _Timer *tmobj = [[_Timer alloc]initWithTimerPtr:tm];
    t = [NSTimer timerWithTimeInterval: i
                            target: tmobj
                            selector:@selector(onTick:)
                            userInfo: nil
                            repeats:YES];
    
    [[NSRunLoop mainRunLoop] addTimer:t forMode:NSRunLoopCommonModes];
    
    [tmobj release];
    registerTimer(tm, t);
}
//-----------------------------------------------------------------------------
void _CocoaTimer::stopTimer(TimerPtr tm) {
    if (!tm) {
        return;
    }
    NSTimer *t = getOsTimer(tm);
    if (!t) {
        return;
    }
    stopTimerImpl(tm, t);
}
//-----------------------------------------------------------------------------
void _CocoaTimer::stopAllTimer() {
    BOOST_FOREACH(TimerMap::value_type &v, timers) {
        stopTimerImpl(v.first, v.second);
    }
}

    
}}} // namesapce(s)

#endif //DISCO_USE_COCOA