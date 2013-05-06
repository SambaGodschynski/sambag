#ifndef SAMBAG__COCOATIMERIMPL_H
#define SAMBAG__COCOATIMERIMPL_H
#ifdef DISCO_USE_COCOA

#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco { namespace components {
class Timer;
//=============================================================================
/**
 * @class _CocoaTimer.
 * Encapsulated cocoa timer implementation.
 */
class _CocoaTimer {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<Timer> TimerPtr;
protected:
    //-------------------------------------------------------------------------
    void startTimer(TimerPtr tm);
    //-------------------------------------------------------------------------
    void stopTimer(TimerPtr tm);
    //-------------------------------------------------------------------------
    static void stopAllTimer();
};
    
}}} // namesapce(s)

#endif //DISCO_USE_COCOA
#endif //SAMBAG__COCOATIMERIMPL_H
