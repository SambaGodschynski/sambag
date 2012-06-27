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
#include <boost/system/error_code.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <sambag/com/ICommand.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class X11WindowToolkit.
  */
class X11WindowToolkit : public WindowToolkit {
//=============================================================================
friend struct Loki::CreateUsingNew<X11WindowToolkit>;
private:
	X11WindowToolkit();
	X11WindowToolkit(const X11WindowToolkit&){}
	//-------------------------------------------------------------------------
	static void mainLoop();
	//-------------------------------------------------------------------------
	typedef boost::asio::deadline_timer TimerImpl;
	typedef sambag::com::ICommand::Ptr CommandPtr;
	typedef boost::bimap<
		boost::bimaps::unordered_set_of<TimerImpl*>,
		boost::bimaps::unordered_set_of<Timer::Ptr> > ToInvoke;
	//-------------------------------------------------------------------------
	static void timerCallback(const boost::system::error_code&,
			TimerImpl* timer, long ms, int repetitions);
	//-------------------------------------------------------------------------
	static ToInvoke toInvoke;
	//-------------------------------------------------------------------------
	static void closeAllTimer();
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
	virtual AWindowPtr createWindowImpl() const;
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	virtual void startTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	virtual void stopTimer(Timer::Ptr tm);
	//-------------------------------------------------------------------------
	static X11WindowToolkit * getToolkit();
	//-------------------------------------------------------------------------
	~X11WindowToolkit();
	//-------------------------------------------------------------------------
	const Globals & getGlobals() const;
	//-------------------------------------------------------------------------
	virtual void startMainLoop();
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const;

}; // X11WindowToolkit
}}} // namespace(s)
#endif // DISCO_USE_X11
#endif /* SAMBAG_X11WINDOWTOOLKIT_H */
