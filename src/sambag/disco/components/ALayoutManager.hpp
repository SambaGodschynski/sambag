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

namespace sambag { namespace disco { namespace components {

class AContainer;
class AComponent;
typedef boost::shared_ptr<AContainer> AContainerPtr;
typedef boost::shared_ptr<AComponent> AComponentPtr;

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
	 * If the layout manager uses a per-component string, adds the component
	 * comp to the layout, associating it with the string specified by name.
	 * @param name
	 * @param comp
	 */
	virtual void
	addLayoutComponent(const std::string & name, AComponentPtr comp) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Lays out the specified container.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Calculates the minimum size dimensions for the specified container,
	 * given the components it contains.
	 * @param parent
	 * @return
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr parent) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Calculates the preferred size dimensions for the specified container,
	 * given the components it contains.
	 * @param parent
	 * @return
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr parent) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified component from the layout.
	 * @param comp
	 */
	virtual void removeLayoutComponent(AComponentPtr comp) = 0;

}; // ALayoutManager
}}} // namespace(s)

#endif /* SAMBAG_ILAYOUTMANAGER_H */
