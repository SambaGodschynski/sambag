/*
 * WindowTestToolkit.hpp
 *
 *  Created on: Mon Jun 18 15:09:40 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WINDOWTESTTOOLKIT_
#define SAMBAG_WINDOWTESTTOOLKIT_

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <loki/Singleton.h>
#include <sambag/disco/components/windowImpl/BoostTimerImpl.hpp>
#include "TestWindowImpl.hpp"
#include <iostream>
#include <boost/thread.hpp>
namespace sambag { namespace disco { namespace components {
//============================================================================
/** 
  * @class WindowTestToolkit.
  */
template <class TimerPolicy = BoostTimerImpl, class _Window = TestWindowImpl>
class WindowTestToolkit : public WindowToolkit, public TimerPolicy{
//=============================================================================
friend struct Loki::CreateUsingNew<WindowTestToolkit>;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<WindowTestToolkit> Ptr;
protected:
	//-------------------------------------------------------------------------
	virtual AWindowImplPtr 
		createWindowImpl(AWindowImplPtr parent = AWindowImplPtr()) const;
private:
	//-------------------------------------------------------------------------
	int mainLoopWaiting;
	//-------------------------------------------------------------------------
	WindowTestToolkit() : mainLoopWaiting(0) {}
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	virtual void invokeLater(const InvokeFunction &f, int ms=50) {
		f(); // call immediately
	}
	//-------------------------------------------------------------------------
	void setMainLoopWaiting( int ms ) {
		mainLoopWaiting = ms;
	}
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const {
		return Dimension(1024, 768);
	}
	//-------------------------------------------------------------------------
	virtual void startMainLoop();
	//-------------------------------------------------------------------------
	virtual void startTimer( Timer::Ptr tm );
	//-------------------------------------------------------------------------
	virtual void stopTimer( Timer::Ptr tm );
	//-------------------------------------------------------------------------
	virtual void quit() {}
	//-------------------------------------------------------------------------
	/**
	 * inits toolkit.
	 * call directly only when no startMainLoop will or was be called.
	 * Eg. when main app is used as nested window.
	 */
	virtual void useWithoutMainloop() {}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new WindowTestToolkit);
	}
}; // WindowTestToolkit
///////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
template <class T, class W>
AWindowImplPtr 
WindowTestToolkit<T, W>::createWindowImpl(AWindowImplPtr parent) const
{
	return W::create();
}
//----------------------------------------------------------------------------
template <class T, class W>
void WindowTestToolkit<T, W>::startMainLoop() {
	enum {Res=100};
	int waited = 0;
	int sec = 1;
	T::startUpTimer();
	while (true) {
		boost::this_thread::sleep(boost::posix_time::millisec(Res));
		waited+=Res;
		if ( waited > 1000 * sec ) {
			++sec;
			std::cout<<"."<<std::flush; // 1 waiting mark per sec.
		}
		if ( waited > mainLoopWaiting )
			break;
	}
	T::tearDownTimer();
}
//-----------------------------------------------------------------------------
template <class T, class W>
void WindowTestToolkit<T, W>::startTimer( Timer::Ptr tm ) {
	T::startTimer(tm);
}
//-----------------------------------------------------------------------------
template <class T, class W>
void WindowTestToolkit<T, W>::stopTimer( Timer::Ptr tm ) {
	T::stopTimer(tm);
}
}}} // namespace(s)

#endif /* SAMBAG_WINDOWTESTTOOLKIT_ */
