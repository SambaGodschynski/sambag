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
#include <sambag/com/ArbitraryType.hpp>

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
	 * @param constraint
	 */
	 virtual void addLayoutComponent(const AComponentSharedOrWeak &comp,
		ArbitraryType::Ptr constraint = ArbitraryType::Ptr()) {}
	//-------------------------------------------------------------------------
	/**
	 * Lays out the specified container.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent) {}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the minimum size dimensions for the specified container,
	 * given the components it contains.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
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
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @return
	 */
	virtual Dimension maximumLayoutSize(AContainerPtr parent) {
		return NULL_DIMENSION;
	}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the preferred size dimensions for the specified container,
	 * given the components it contains.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
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
	//-------------------------------------------------------------------------
	/**
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentY(AContainerPtr target) {
		return Coordinate(NULL_NUMBER);
	}
	//-------------------------------------------------------------------------
	/**
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentX(AContainerPtr target) {
		return Coordinate(NULL_NUMBER);
	}
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return "ALayoutManager";
	}

}; // ALayoutManager
}}} // namespace(s)

#endif /* SAMBAG_ILAYOUTMANAGER_H */
