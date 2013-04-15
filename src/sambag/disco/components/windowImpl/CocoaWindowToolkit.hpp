/*
 * Win32WindowToolkit.hpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COCOAWINDOWTOOLKIT_H
#define SAMBAG_COCOAWINDOWTOOLKIT_H

#ifdef DISCO_USE_COCOA
#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <loki/Singleton.h>
#include "BoostTimerImpl.hpp"
#include "NullTimerImpl.hpp"

namespace sambag { namespace disco { namespace components {

//typedef Win32TimerImpl TimerImpl;
//typedef Win32MMTimerImpl TimerImpl;
typedef BoostTimerImpl TimerImpl;

//=============================================================================
/** 
  * @class CocoaWindowToolkit.
  */
class CocoaWindowToolkit : public WindowToolkit,
						   public TimerImpl
{
//=============================================================================
friend struct Loki::CreateUsingNew<CocoaWindowToolkit>;
private:
	//-------------------------------------------------------------------------
	typedef TimerImpl TimerPolicy;
	//-------------------------------------------------------------------------
	CocoaWindowToolkit();
	CocoaWindowToolkit(const CocoaWindowToolkit&){}
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
	static CocoaWindowToolkit * getToolkit();
	//-------------------------------------------------------------------------
	~CocoaWindowToolkit();
	//-------------------------------------------------------------------------
	virtual void startMainLoop();
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const;
	//-------------------------------------------------------------------------
	virtual void quit();
}; // CocoaWindowToolkit
}}} // namespace(s)
#endif // DISCO_USE_COCOA
#endif /* SAMBAG_COCOAWINDOWTOOLKIT_H */
