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
	virtual void invokeLater(const InvokeFunction &f) = 0;
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
