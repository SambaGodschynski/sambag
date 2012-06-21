/*
 * X11WindowToolkit.hpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_X11WINDOWTOOLKIT_H
#define SAMBAG_X11WINDOWTOOLKIT_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <loki/Singleton.h>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class X11WindowToolkit.
  */
class X11WindowToolkit : public WindowToolkit {
//=============================================================================
friend struct Loki::CreateUsingNew<X11WindowToolkit>;
private:
	X11WindowToolkit(){}
	X11WindowToolkit(const X11WindowToolkit&){}
public:
	//-------------------------------------------------------------------------
	virtual void startMainLoop();
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const;

}; // X11WindowToolkit
}}} // namespace(s)

#endif /* SAMBAG_X11WINDOWTOOLKIT_H */
