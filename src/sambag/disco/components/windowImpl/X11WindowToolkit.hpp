/*
 * X11WindowToolkit.hpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_X11WINDOWTOOLKIT_H
#define SAMBAG_X11WINDOWTOOLKIT_H

#ifdef DISCO_USE_X11
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <loki/Singleton.h>
#include <X11/Xlib.h>
#include <sambag/com/ICommand.hpp>
#include "BoostTimerImpl.hpp"
#include <queue>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class X11WindowToolkit.
  */
class X11WindowToolkit :public WindowToolkit,
						 public BoostTimerImpl //it's possible to set
						                        //the BoostTimer as template
						                        //policy, but the hassle that
						                        //comes with templates makes my
						                        //decision to use simple
						                        //inheritance.
{
//=============================================================================
friend struct Loki::CreateUsingNew<X11WindowToolkit>;
private:
	//-------------------------------------------------------------------------
	typedef BoostTimerImpl TimerPolicy;
	//-------------------------------------------------------------------------
	X11WindowToolkit();
	X11WindowToolkit(const X11WindowToolkit&){}
	//-------------------------------------------------------------------------
	static void mainLoop();
	//-------------------------------------------------------------------------
	static void invokeWaiting();
	//-------------------------------------------------------------------------
	typedef std::queue<InvokeFunction> InvokeLater;
	//-------------------------------------------------------------------------
	static InvokeLater _invokeLater;
	//-------------------------------------------------------------------------
	static bool mainLoopRunning;
public:
	//-------------------------------------------------------------------------
	struct Globals {
		::Display * display;
		Globals() : display(NULL) {}
	};
protected:
	//-------------------------------------------------------------------------
	Globals globals;
	//-------------------------------------------------------------------------
	virtual AWindowImplPtr
	createWindowImpl(AWindowImplPtr parent = AWindowImplPtr()) const;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	/**
	 * aborts mainloop.
	 */
	virtual void quit();
	//-------------------------------------------------------------------------
	virtual void useWithoutMainloop() {}
	//-------------------------------------------------------------------------
	/**
	 * inits toolkit.
	 * call directly only when no startMainLoop will or was be called.
	 * Eg. when main app is used as nested window.
	 */
	virtual void initToolkit() {}
	//-------------------------------------------------------------------------
	static bool isMainLoopRunning() {
		return mainLoopRunning;
	}
	//-------------------------------------------------------------------------
    /**
	 * @note delay and tracking not supported.
	 */
        virtual void invokeLater(const InvokeFunction &f, int delay=50,
        boost::shared_ptr<void> toTrack=boost::shared_ptr<void>());
	//-------------------------------------------------------------------------
	virtual void startTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	virtual void stopTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	static X11WindowToolkit * getToolkit();
	//-------------------------------------------------------------------------
	~X11WindowToolkit();
	//-------------------------------------------------------------------------
	/**
	 * X11 specific
	 **/
	const Globals & getGlobals() const;
	//-------------------------------------------------------------------------
	virtual void startMainLoop();
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const;
}; // X11WindowToolkit
}}} // namespace(s)
#endif // DISCO_USE_X11
#endif /* SAMBAG_X11WINDOWTOOLKIT_H */
