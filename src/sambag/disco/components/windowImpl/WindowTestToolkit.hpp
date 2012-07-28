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

namespace sambag { namespace disco { namespace components {

//============================================================================
/** 
  * @class WindowTestToolkit.
  */
class WindowTestToolkit : public WindowToolkit {
//=============================================================================
friend struct Loki::CreateUsingNew<WindowTestToolkit>;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<WindowTestToolkit> Ptr;
protected:
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const;
private:
	//-------------------------------------------------------------------------
	WindowTestToolkit() {}
public:
	//-------------------------------------------------------------------------
	virtual Dimension getScreenSize() const;
	//-------------------------------------------------------------------------
	virtual void startMainLoop() {}
	//-------------------------------------------------------------------------
	virtual void startTimer( Timer::Ptr tm ) {
	}
	//-------------------------------------------------------------------------
	virtual void stopTimer( Timer::Ptr tm ) {

	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new WindowTestToolkit);
	}
}; // WindowTestToolkit
}}} // namespace(s)

#endif /* SAMBAG_WINDOWTESTTOOLKIT_ */
