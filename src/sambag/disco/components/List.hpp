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
#include <string>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class List.
  * Unfortunately it is not possible to use a generic List because it isn't
  * possible to register a generic class with a LookAndFeel's registerClass.
  * So every specific list type has to be registered manually.
  */
template <typename T>
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
	// Compiler Error C3200 on MSVC. ? WTF
	// see: http://msdn.microsoft.com/de-de/library/3xwxftta.aspx
	/*typedef AList< T,
			DefaultListCellRenderer,
			DefaultListModel,
			DefaultListSelectionModel> Super;*/
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const
	{
		return laf->getUI<Class> ();
	}
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
////////////////////////////////////////////////////////////////////////////////
typedef List<std::string> StringList;
}}} // namespace(s)

#endif /* SAMBAG_LIST_H */
