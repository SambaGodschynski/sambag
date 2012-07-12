/*
 * BasicLabelUI.hpp
 *
 *  Created on: Tue Jun 12 17:23:39 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICLABELUI_H
#define SAMBAG_BASICLABELUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicLabelUI.
  */
class BasicLabelUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AComponentUI Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicLabelUI> Ptr;
protected:
	//-------------------------------------------------------------------------
	BasicLabelUI(){}
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new BasicLabelUI());
	}
	//-------------------------------------------------------------------------
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual Dimension getPreferredSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual Dimension getMaximumSize(AComponentPtr c) {
		return getPreferredSize(c);
	}
	//-------------------------------------------------------------------------
	virtual Dimension getMinimumSize(AComponentPtr c) {
		return getPreferredSize(c);
	}
}; // BasicLabelUI
}}}}} // namespace(s)

#endif /* SAMBAG_BASICLABELUI_H */
