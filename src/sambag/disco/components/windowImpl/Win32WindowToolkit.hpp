/*
 * Win32WindowToolkit.hpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32WINDOWTOOLKIT_H
#define SAMBAG_WIN32WINDOWTOOLKIT_H

#ifdef DISCO_USE_WIN32
#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <loki/Singleton.h>
#include "Win32TimerImpl.hpp"
#include "Win32MMTimerImpl.hpp"
#include "BoostTimerImpl.hpp"
#include "NullTimerImpl.hpp"

namespace sambag { namespace disco { namespace components {

typedef Win32TimerImpl TimerImpl;
//typedef Win32MMTimerImpl TimerImpl;
//typedef BoostTimerImpl TimerImpl;

//=============================================================================
/** 
  * @class Win32WindowToolkit.
  */
class Win32WindowToolkit : public WindowToolkit,
						   public TimerImpl
{
//=============================================================================
friend struct Loki::CreateUsingNew<Win32WindowToolkit>;
private:
	//-------------------------------------------------------------------------
	typedef TimerImpl TimerPolicy;
	//-------------------------------------------------------------------------
	Win32WindowToolkit();
	Win32WindowToolkit(const Win32WindowToolkit&){}
	//-------------------------------------------------------------------------
	static bool mainLoopRunning;
	//-------------------------------------------------------------------------
	static void mainLoop();
public:
protected:
	//-------------------------------------------------------------------------
	void onRefresh();
	//-------------------------------------------------------------------------
	virtual AWindowImplPtr createWindowImpl(AWindowImplPtr parent = AWindowImplPtr()) const;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	/**
	 * inits toolkit.
	 * call directly only when no startMainLoop will or was be called.
	 * Eg. when main app is used as nested window.
	 */
	virtual void useWithoutMainloop();
	//-------------------------------------------------------------------------
	virtual WindowPtr createNestedWindow( ArbitraryType::Ptr osParent, 
		const Rectangle &area );
	//-------------------------------------------------------------------------
	static bool isMainLoopRunning() {
		return mainLoopRunning;
	}
	//-------------------------------------------------------------------------
	virtual void invokeLater(const InvokeFunction &f);
	//-------------------------------------------------------------------------
	virtual void invokeLater(const InvokeFunction &f, int ms);
	//-------------------------------------------------------------------------
	virtual void startTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	virtual void stopTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	static Win32WindowToolkit * getToolkit();
	//-------------------------------------------------------------------------
	~Win32WindowToolkit();
	//-------------------------------------------------------------------------
	virtual void startMainLoop();
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const;
	//-------------------------------------------------------------------------
	virtual void quit();
}; // Win32WindowToolkit
}}} // namespace(s)
#endif // DISCO_USE_WIN32
#endif /* SAMBAG_WIN32WINDOWTOOLKIT_H */
