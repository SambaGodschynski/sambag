/*
 * TestWindowFactory.hpp
 *
 *  Created on: Mon Jun 18 15:09:40 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TESTWINDOWFACTORY_H
#define SAMBAG_TESTWINDOWFACTORY_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>

namespace sambag { namespace disco { namespace components {

//============================================================================
/** 
  * @class TestWindowFactory.
  */
class TestWindowFactory : public WindowToolkit {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<TestWindowFactory> Ptr;
protected:
private:
	//-------------------------------------------------------------------------
	TestWindowFactory() {}
public:
	//-------------------------------------------------------------------------
	virtual void startMainLoop() {}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new TestWindowFactory);
	}
	//-------------------------------------------------------------------------
	virtual AWindowPtr createWindowImpl() const;
}; // TestWindowFactory
}}} // namespace(s)

#endif /* SAMBAG_TESTWINDOWFACTORY_H */
