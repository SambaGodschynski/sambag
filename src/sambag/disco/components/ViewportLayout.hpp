/*
 * ViewportLayout.hpp
 *
 *  Created on: Sat Jun 23 19:30:18 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_VIEWPORTLAYOUT_H
#define SAMBAG_VIEWPORTLAYOUT_H

#include <boost/shared_ptr.hpp>
#include "ALayoutManager.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ViewportLayout.
  */
class ViewportLayout : public ALayoutManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ViewportLayout> Ptr;
protected:
	//-------------------------------------------------------------------------
	ViewportLayout();
private:
	//-------------------------------------------------------------------------
	// Single instance used by JViewport.
	static Ptr SHARED_INSTANCE;
public:
	//-------------------------------------------------------------------------
	static Ptr getSharedInstance();
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new ViewportLayout());
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * Adds the specified component to the layout.
	 * @param name
	 * @param c
	 */
	virtual void addLayoutComponent(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Called by the AWT when the specified container needs to be laid out.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Returns the minimum dimensions needed to layout the components contained
	 * in the specified target container.
	 * @param parent
	 * @return
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Returns the preferred dimensions for this layout given the
	 * components in the specified target container.
	 * @param parent
	 * @return
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified component from the layout.
	 * @param c
	 */
	virtual void removeLayoutComponent(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return "ViewportLayout";
	}
}; // ViewportLayout
}}} // namespace(s)

#endif /* SAMBAG_VIEWPORTLAYOUT_H */
