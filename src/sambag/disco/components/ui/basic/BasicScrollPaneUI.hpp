/*
 * BasicScrollPaneUI.hpp
 *
 *  Created on: Sun Jul 15 10:46:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICSCROLLPANEUI_H
#define SAMBAG_BASICSCROLLPANEUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/ScrollPane.hpp>
#include <sambag/disco/components/Viewport.hpp>
#include <sambag/disco/components/Forward.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicScrollPaneUI.
  */
class BasicScrollPaneUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AComponentUI Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicScrollPaneUI> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<BasicScrollPaneUI> WPtr;
protected:
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	BasicScrollPaneUI(){}
	//-------------------------------------------------------------------------
	void syncScrollPaneWithViewport();
private:
	//-------------------------------------------------------------------------
	ScrollPaneWPtr _scrollpane;
	//-------------------------------------------------------------------------
	Coordinate oldExtent;
public:
	//-------------------------------------------------------------------------
	ScrollPanePtr getScrollPane() const {
		return _scrollpane.lock();
	}
	//-------------------------------------------------------------------------
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new BasicScrollPaneUI());
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	void vsbStateChanged(void *src,
		const ScrollPane::ScrollbarModel::StateChangedEvent &ev);
	//-------------------------------------------------------------------------
	void hsbStateChanged(void *src,
		const ScrollPane::ScrollbarModel::StateChangedEvent &ev);
	//-------------------------------------------------------------------------
	void viewportStateChanged(void *src,
			const Viewport::StateChangedEvent &ev);
}; // BasicScrollPaneUI
}}}}} // namespace(s)
#endif /* SAMBAG_BASICSCROLLPANEUI_H */
