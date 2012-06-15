/*
 * IMenuElement.hpp
 *
 *  Created on: Thu Jun 14 12:57:49 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IMENUELEMENT_H
#define SAMBAG_IMENUELEMENT_H

#include <boost/shared_ptr.hpp>
#include <vector>
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @interface IMenuElement.
  * Any component that can be placed into a menu should implement this interface.
  * This interface is used by MenuSelectionManager to handle selection and
  * navigation in menu hierarchies.
  */
class IMenuElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::vector<IMenuElement> MenuElements;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IMenuElement> Ptr;
	//-------------------------------------------------------------------------
	/**
	 * @return return the Component used to paint the receiving element.
	 */
	virtual AComponentPtr getComponent() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * This method should return an array containing the sub-elements for
	 * the receiving menu element
	 */
	virtual const MenuElements & getSubElements() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Call by the MenuSelectionManager when the MenuElement is added or
	 * remove from the menu selection.
	 */
	virtual void menuSelectionChanged(bool isIncluded) = 0;
}; // IMenuElement
}}} // namespace(s)

#endif /* SAMBAG_IMENUELEMENT_H */
