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
#include <sambag/com/Common.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class DefaultListCellRenderer.
  * Default renderer for AList. Using Label as rendering component. T has to
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
		setOpaque(true);
	}
private:
public:
	//-------------------------------------------------------------------------
	/**
	 * @override makes installLookAndFeel() accessible for AList<>.
	 * @param laf
	 */
	virtual void installLookAndFeel(ui::ALookAndFeelPtr laf) {
		Super::installLookAndFeel(laf);
	}
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(DefaultListCellRenderer)
	//-------------------------------------------------------------------------
	template <class ListType>
	AComponentPtr getListCellRendererComponent(
			boost::shared_ptr<ListType> list, // the list
			const T &value, // value to display
			int index, // cell index
			bool isSelected, // is the cell selected
			bool cellHasFocus
	); // does the cell have focus
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 */
	void validate() {}
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 *
	 * @since 1.5
	 */
	void invalidate() {}
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 *
	 * @since 1.5
	 */
	void redraw() {}
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 */
	void revalidate() {}
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 */
	void redraw(const Rectangle &r) {}
	//-------------------------------------------------------------------------
protected:
	/*void firePropertyChange(String propertyName, Object oldValue, Object newValue) {
	 // Strings get interned...
	 if (propertyName == "text"
	 || ((propertyName == "font" || propertyName == "foreground")
	 && oldValue != newValue
	 && getClientProperty(javax.swing.plaf.basic.BasicHTML.propertyKey) != null)) {

	 super.firePropertyChange(propertyName, oldValue, newValue);
	 }
	 }*/
}; // DefaultListCellRenderer
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class T>
template <class ListType>
AComponentPtr DefaultListCellRenderer<T>::getListCellRendererComponent(
		boost::shared_ptr<ListType> list, const T &value, int index,
		bool isSelected, bool cellHasFocus)
{
	//setComponentOrientation(list.getComponentOrientation());

	if (isSelected) {
		Super::setBackground(list->getSelectionBackground());
		Super::setForeground(list->getSelectionForeground());
	} else {
		Super::setBackground(list->getBackground());
		Super::setForeground(list->getForeground());
	}

	setText(sambag::com::toString(value));

	Super::setEnabled(list->isEnabled());
	Super::setFont(list->getFont());

	return getPtr();

}
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTLISTCELLRENDERER_H */
