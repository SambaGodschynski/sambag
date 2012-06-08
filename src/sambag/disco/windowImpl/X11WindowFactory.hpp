/*
 * X11WindowFactory.hpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_X11WINDOWFACTORY_H
#define SAMBAG_X11WINDOWFACTORY_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/IWindowFactory.hpp>
#include <loki/Singleton.h>

namespace sambag { namespace disco {

//=============================================================================
/** 
  * @class X11WindowFactory.
  */
class X11WindowFactory : public IWindowFactory {
//=============================================================================
friend struct Loki::CreateUsingNew<X11WindowFactory>;
private:
	X11WindowFactory(){}
	X11WindowFactory(const X11WindowFactory&){}
public:
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const;

}; // X11WindowFactory
}} // namespace(s)

#endif /* SAMBAG_X11WINDOWFACTORY_H */
