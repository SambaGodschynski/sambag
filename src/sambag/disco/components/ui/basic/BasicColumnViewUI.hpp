/*
 * BasicColumnViewUI.hpp
 *
 *  Created on: Mon Sep 17 17:39:33 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICCOLUMNVIEWUI_H
#define SAMBAG_BASICCOLUMNVIEWUI_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>

namespace sambag { namespace disco { namespace components {
namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicColumnViewUI.
  */
class BasicColumnViewUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AComponentUI Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicColumnViewUI> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<BasicColumnViewUI> WPtr;
protected:
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	BasicColumnViewUI();
private:
	//-------------------------------------------------------------------------
	void installDefaults(AComponentPtr c);
public:
	//-------------------------------------------------------------------------
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new BasicColumnViewUI());
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
}; // BasicColumnViewUI
}}}}} // namespace(s)

#endif /* SAMBAG_BASICCOLUMNVIEWUI_H */
