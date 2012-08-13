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
#include "BoostTimerImpl.hpp"
#include "NullTimerImpl.hpp"
#include <queue>

namespace sambag { namespace disco { namespace components {

//typedef Win32TimerImpl TimerImpl;
typedef BoostTimerImpl TimerImpl;

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
	static void invokeWaiting();
	//-------------------------------------------------------------------------
	typedef TimerImpl TimerPolicy;
	//-------------------------------------------------------------------------
	typedef std::queue<InvokeFunction> InvokeLater;
	//-------------------------------------------------------------------------
	static InvokeLater _invokeLater;
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
	static bool isMainLoopRunning() {
		return mainLoopRunning;
	}
	//-------------------------------------------------------------------------
	virtual void invokeLater(const InvokeFunction &f);
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

}; // Win32WindowToolkit
}}} // namespace(s)
#endif // DISCO_USE_WIN32
#endif /* SAMBAG_WIN32WINDOWTOOLKIT_H */
