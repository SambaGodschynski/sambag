/*
 * WindowToolkit.hpp
 *
 *  Created on: Thu Jun  7 10:44:45 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WINDOWTOOLKIT_H
#define SAMBAG_WINDOWTOOLKIT_H

#include <boost/shared_ptr.hpp>
#include "Forward.hpp"
#include <sambag/disco/Geometry.hpp>
#include <sambag/com/ICommand.hpp>
#include "Timer.hpp"
#include <boost/function.hpp>
#include <sambag/com/ArbitraryType.hpp>
#include <sambag/com/Common.hpp>
#include <stdexcept>
#include <sambag/disco/components/events/MouseEventCreator.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class WindowToolkit.
  */
class WindowToolkit {
//=============================================================================
friend class Window;
protected:
	//-------------------------------------------------------------------------
	virtual AWindowImplPtr 
	createWindowImpl(AWindowImplPtr parent = AWindowImplPtr()) const = 0;
	//-------------------------------------------------------------------------
	/**
	 * holds window shared ptr until window is closed.
	 */
	virtual void holdWindowPtr(WindowPtr win);
private:
    //-------------------------------------------------------------------------
    events::MouseEventCreator::Ptr mecPrototype;
public:
	//-------------------------------------------------------------------------
	/**
	 * Creates a Window that uses an existing Window as host. 
	 * @param osParent depends on OS:
	 *		Win32: pair<HWND, HINSTANCE>
	 * @param area the area of the host window which has to be used by 
	 *             the new window (not every OS need this)
	 */ 
	virtual WindowPtr createNestedWindow( ArbitraryType::Ptr osParent, 
		const Rectangle &area ) 
	{
		throw std::logic_error(
			"createNestedWindow is not impl. with this WindowToolkit."
		);
		return WindowPtr();
	}
	//-------------------------------------------------------------------------
	virtual void startTimer( Timer::Ptr tm ) = 0;
	//-------------------------------------------------------------------------
	virtual void stopTimer( Timer::Ptr tm ) = 0;
	//-------------------------------------------------------------------------
	virtual void startMainLoop() = 0;
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const = 0;
	//-------------------------------------------------------------------------
	typedef boost::function<void()> InvokeFunction;
	//-------------------------------------------------------------------------
	/**
     * calls f after delay in ms.
     * @param the function to call
     * @param the minimum of time in ms which has to pass. 
     *        (not all impl. support this)
     */
    virtual void invokeLater(const InvokeFunction &f, int delay=50) = 0;
	//-------------------------------------------------------------------------
	/**
	 * aborts mainloop.
	 */
	virtual void quit() = 0;
	//-------------------------------------------------------------------------
	/**
	 * inits toolkit.
	 * call directly only when no startMainLoop will or was be called.
	 * Eg. when main app is used as nested window.
	 */
	virtual void useWithoutMainloop() = 0;
    //-------------------------------------------------------------------------
    /**
     * sets the mouse event creator which is the prototype for all
     * crated windows in future.
     * @arg the prototype object
     * 
     */
    virtual void setMouseEventCreatorPrototype(events::MouseEventCreator::Ptr obj);
    //-------------------------------------------------------------------------
    /**
     * return the mouse event creator which is the prototype for all
     * crated windows in future.
     */
    virtual events::MouseEventCreator::Ptr getMouseEventCreatorPrototype();
}; // WindowToolkit
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
extern WindowToolkit * _getWindowToolkitImpl();
//-----------------------------------------------------------------------------
WindowToolkit * getWindowToolkit();
//-----------------------------------------------------------------------------
void setGlobalUserData(const std::string &key, sambag::com::ArbitraryType::Ptr);
//-----------------------------------------------------------------------------
sambag::com::ArbitraryType::Ptr getGlobalUserData(const std::string &key);
/**
 * used for testing
 * @param current WindowToolkit
 */
void setWindowToolkit(WindowToolkit *wf);
}}} // namespace(s)
#endif /* SAMBAG_WINDOWTOOLKIT_H */
