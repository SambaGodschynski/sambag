/*
 * List.hpp
 *
 *  Created on: Fri Sep  7 12:36:51 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LIST_H
#define SAMBAG_LIST_H

#include <boost/shared_ptr.hpp>
#include "AList.hpp"
#include "AComponent.hpp"
#include "DefaultListCellRenderer.hpp"
#include "DefaultListModel.hpp"
#include "DefaultListSelectionModel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class List.
  */
template <class T>
class List : public AList<
		T,
		DefaultListCellRenderer,
		DefaultListModel,
		DefaultListSelectionModel
	>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef List<T> Class;
	//-------------------------------------------------------------------------
	typedef AList<T,
			DefaultListCellRenderer,
			DefaultListModel,
			DefaultListSelectionModel> Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
protected:
	//-------------------------------------------------------------------------
	List() {
		Super::setName("List");
	}
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(List)
}; // List
}}} // namespace(s)

#endif /* SAMBAG_LIST_H */
