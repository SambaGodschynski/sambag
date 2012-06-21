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

namespace sambag { namespace disco { namespace components {

//============================================================================
/** 
  * @class WindowTestToolkit.
  */
class WindowTestToolkit : public WindowToolkit {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<WindowTestToolkit> Ptr;
protected:
private:
	//-------------------------------------------------------------------------
	WindowTestToolkit() {}
public:
	//-------------------------------------------------------------------------
	virtual void startMainLoop() {}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new WindowTestToolkit);
	}
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const;
}; // WindowTestToolkit
}}} // namespace(s)

#endif /* SAMBAG_WINDOWTESTTOOLKIT_ */
