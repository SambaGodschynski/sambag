/*
 * DefaultListCellRenderer.hpp
 *
 *  Created on: Thu Sep  6 14:27:02 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTLISTCELLRENDERER_H
#define SAMBAG_DEFAULTLISTCELLRENDERER_H

#include <boost/shared_ptr.hpp>
#include "Label.hpp"
#include "ui/ALookAndFeel.hpp"
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class DefaultListCellRenderer.
  * Default renderer for List. Using Label as rendering component. T has to
  * be convertible to using the ostream<<T operator.
  */
template <class T>
class DefaultListCellRenderer : public Label {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef T CellContentType;
	//-------------------------------------------------------------------------
	typedef Label Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<DefaultListCellRenderer> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr
	getComponentUI(ui::ALookAndFeelPtr laf) const {
		return ui::AComponentUIPtr();
	}
protected:
	//-------------------------------------------------------------------------
	DefaultListCellRenderer() {
		setName("DefaultListCellRenderer");
	}
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(DefaultListCellRenderer)
	//-------------------------------------------------------------------------
	virtual AComponentPtr getListCellRendererComponent(
			AContainerPtr list,           // the list
			const T &value, // value to display
			int index, // cell index
			bool isSelected, // is the cell selected
			bool cellHasFocus) // does the cell have focus
	{
		return AComponentPtr();
	}
}; // DefaultListCellRenderer
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTLISTCELLRENDERER_H */
