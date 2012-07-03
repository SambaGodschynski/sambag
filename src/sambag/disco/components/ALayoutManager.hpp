/*
 * ALayoutManager.hpp
 *
 *  Created on: Wed May 16 12:52:12 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ILAYOUTMANAGER_H
#define SAMBAG_ILAYOUTMANAGER_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/Geometry.hpp>
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ALayoutManager.
  */
class ALayoutManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ALayoutManager> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<ALayoutManager> WPtr;
protected:
	//-------------------------------------------------------------------------
	/*
	 * setted during cretate() by subclasses
	 */
	WPtr self;
	//-------------------------------------------------------------------------
	ALayoutManager();
private:
public:
	//-------------------------------------------------------------------------
	virtual Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	virtual ~ALayoutManager();
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param comp
	 * @param c
	 */	template<class LayoutMng, class Constraint>
	void addLayoutComponent(AComponentPtr comp, const Constraint &c) {
		boost::shared_ptr<LayoutMng> lm =
				boost::shared_dynamic_cast<LayoutMng>(getPtr());
		if (!lm) {
			// layout manager dosen't match add without constraint
			addLayoutComponent(comp);
			return;
		}
		lm->LayoutMng::addLayoutComponentImpl(comp, c);
	}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param comp
	 */
	 virtual void addLayoutComponent(AComponentPtr comp) {}
	//-------------------------------------------------------------------------
	/**
	 * Lays out the specified container.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent) {}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the minimum size dimensions for the specified container,
	 * given the components it contains.
	 * @param parent
	 * @return
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr parent) {
		return NULL_DIMENSION;
	}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the maximum size dimensions for the specified container,
	 * given the components it contains.
	 * @param parent
	 * @return
	 */
	virtual Dimension maximumLayoutSize(AContainerPtr parent) {
		return NULL_DIMENSION;
	}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the preferred size dimensions for the specified container,
	 * given the components it contains.
	 * @param parent
	 * @return
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr parent) {
		return NULL_DIMENSION;
	}
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified component from the layout.
	 * @param comp
	 */
	virtual void removeLayoutComponent(AComponentPtr comp) {}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param comp
	 */
	virtual void invalidateLayout(AContainerPtr target) {}
	//-----------------------------------------------------------------------------
	virtual Coordinate getLayoutAlignmentY(AContainerPtr target) {
		return Coordinate(NULL_NUMBER);
	}
	//-----------------------------------------------------------------------------
	virtual Coordinate getLayoutAlignmentX(AContainerPtr target) {
		return Coordinate(NULL_NUMBER);
	}
	//-----------------------------------------------------------------------------
	virtual std::string toString() const {
		return "ALayoutManager";
	}

}; // ALayoutManager
}}} // namespace(s)

#endif /* SAMBAG_ILAYOUTMANAGER_H */
