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
#include <loki/Singleton.h>
#include "Win32TimerImpl.hpp"
// including windows.h causes 118 errors because of redef.
#include "BoostTimerImpl.hpp"
namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Win32WindowToolkit.
  */
class Win32WindowToolkit : public WindowToolkit,
						   public Win32TimerImpl
{
//=============================================================================
friend struct Loki::CreateUsingNew<Win32WindowToolkit>;
private:
	//-------------------------------------------------------------------------
	typedef Win32TimerImpl TimerPolicy;
	//-------------------------------------------------------------------------
	Win32WindowToolkit();
	Win32WindowToolkit(const Win32WindowToolkit&){}
	//-------------------------------------------------------------------------
	static void mainLoop();
public:
protected:
	//-------------------------------------------------------------------------
	virtual AWindowImplPtr createWindowImpl(AWindowImplPtr parent = AWindowImplPtr()) const;
	//-------------------------------------------------------------------------
public:
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
