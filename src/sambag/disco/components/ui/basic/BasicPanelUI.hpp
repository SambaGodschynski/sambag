/*
 * BasicPanelUI.hpp
 *
 *  Created on: Thu Jul 12 14:51:08 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICPANELUI_H
#define SAMBAG_BASICPANELUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicPanelUI.
  */
class BasicPanelUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicPanelUI> Ptr;
protected:
	//-------------------------------------------------------------------------
	BasicPanelUI() {}
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new BasicPanelUI());
	}
}; // BasicPanelUI
}}}}} // namespace(s)

#endif /* SAMBAG_BASICPANELUI_H */
