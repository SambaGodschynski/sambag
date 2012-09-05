/*
 * List.hpp
 *
 *  Created on: Thu Jul  5 16:54:31 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LIST_H
#define SAMBAG_LIST_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include "ui/ALookAndFeel.hpp"
#include "IScrollable.hpp"
#include "DefaultListModel.hpp"
#include "DefaultListSelectionModel.hpp"
namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
 * @class List.
 */
template <
	class T,
	class _ListSelectionModel = DefaultListSelectionModel,
	template <class> class _ListModel = DefaultListModel
>
class List: public AContainer,
		public _ListModel<T>,
		public _ListSelectionModel
		//public IScrollable
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _ListModel<T> ListModel;
	//-------------------------------------------------------------------------
	typedef _ListSelectionModel ListSelectionModel;
	//-------------------------------------------------------------------------
	typedef List<T, _ListSelectionModel, _ListModel> Class;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<List> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const
	{
		return laf->getUI<List> ();
	}
protected:
	//-------------------------------------------------------------------------
	List() {
		setName("List");
	}
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(List)
	//-------------------------------------------------------------------------
//	/**
//	 * Sets the selection to be the union of the specified interval with
//	 * current selection.
//	 * @param anchor
//	 * @param lead
//	 */
//	void addSelectionInterval(int anchor, int lead);
//	//-------------------------------------------------------------------------
//	/**
//	 * Clears the selection; after calling this method, isSelectionEmpty
//	 *  will return true.
//	 */
//	void clearSelection();
protected:
	//-------------------------------------------------------------------------
	std::string paramString();
public:
//	//-------------------------------------------------------------------------
//	/**
//	 * Scrolls the list within an enclosing viewport to make the
//	 * specified cell completely visible.
//	 * @param index
//	 */
//	virtual void ensureIndexIsVisible(int index);
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the anchor selection index.
//	 * @return
//	 */
//	virtual int getAnchorSelectionIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the bounding rectangle, in the list's coordinate system,
//	 * for the range of cells specified by the two indices.
//	 * @param index0
//	 * @param index1
//	 * @return
//	 */
//	virtual Rectangle getCellBounds(int index0, int index1) const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the object responsible for painting list items.
//	 */
//	// TODO: ListCellRenderer<? super E>	getCellRenderer() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the smallest list index that is currently visible.
//	 * @return
//	 */
//	virtual int getFirstVisibleIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the value of the fixedCellHeight property.
//	 * @return
//	 */
//	virtual Coordinate getFixedCellHeight() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the value of the fixedCellWidth property.
//	 * @return
//	 */
//	virtual Coordinate getFixedCellWidth() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the largest list index that is currently visible.
//	 * @return
//	 */
//	virtual int getLastVisibleIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the layout orientation property for the list: VERTICAL if the
//	 * layout is a single column of cells, VERTICAL_WRAP if the layout is
//	 * "newspaper style" with the content flowing vertically then horizontally,
//	 *  or HORIZONTAL_WRAP if the layout is "newspaper style" with the content
//	 *   flowing horizontally then vertically.
//	 * @return
//	 */
//	virtual int getLayoutOrientation() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * 	Returns the lead selection index.
//	 * @return
//	 */
//	virtual int getLeadSelectionIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the largest selected cell index, or -1 if the selection is empty.
//	 * @return
//	 */
//	virtual int getMaxSelectionIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the smallest selected cell index, or -1 if the selection is empty.
//	 * @return
//	 */
//	virtual int getMinSelectionIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Computes the size of viewport needed to display visibleRowCount rows.
//	 * @return
//	 */
//	virtual Dimension getPreferredScrollableViewportSize() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the "prototypical" cell value -- a value used to calculate a
//	 * fixed width and height for cells.
//	 * @return
//	 */
//	virtual const T & getPrototypeCellValue() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the distance to scroll to expose the next or previous block.
//	 * @param visibleRect
//	 * @param orientation
//	 * @param direction
//	 * @return
//	 */
//	virtual Coordinate
//	getScrollableBlockIncrement(Rectangle visibleRect, int orientation,
//			int direction) const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns true if this JList is displayed in a JViewport and the viewport
//	 * is taller than the list's preferred height, or if the layout orientation
//	 * is VERTICAL_WRAP and visibleRowCount <= 0; otherwise returns false.
//	 * @return
//	 */
//	virtual bool getScrollableTracksViewportHeight() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns true if this JList is displayed in a JViewport and the viewport
//	 * is wider than the list's preferred width, or if the layout orientation is
//	 * HORIZONTAL_WRAP and visibleRowCount <= 0; otherwise returns false.
//	 * @return
//	 */
//	virtual bool getScrollableTracksViewportWidth() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the distance to scroll to expose the next or previous row
//	 * (for vertical scrolling) or column (for horizontal scrolling).
//	 * @param visibleRect
//	 * @param orientation
//	 * @param direction
//	 * @return
//	 */
//	virtual Coordinate
//	getScrollableUnitIncrement(Rectangle visibleRect, int orientation,
//			int direction) const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the smallest selected cell index; the selection when only a
//	 * single item is selected in the list.
//	 * @return
//	 */
//	virtual int getSelectedIndex() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns an array of all of the selected indices, in increasing order.
//	 */
//	template<class IndexContainer>
//	void getSelectedIndices(IndexContainer &out) const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the value for the smallest selected cell index; the selected
//	 * value when only a single item is selected in the list.
//	 * @return
//	 */
//	virtual const T & getSelectedValue() const;
//	//-------------------------------------------------------------------------
//	/**
//	 *
//	 */
//	template<class Container>
//	void getSelectedValues(Container &out) const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the color used to draw the background of selected items.
//	 * @return
//	 */
//	virtual ColorRGBA getSelectionBackground() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the color used to draw the foreground of selected items.
//	 * @return
//	 */
//	virtual ColorRGBA getSelectionForeground() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the current selection mode for the list.
//	 * @return
//	 */
//	virtual int getSelectionMode() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the value of the visibleRowCount property.
//	 * @return
//	 */
//	int getVisibleRowCount() const;
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the origin of the specified item in the list's coordinate system.
//	 * @param index
//	 * @return
//	 */
//	virtual Point2D indexToLocation(int index);
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns true if the specified index is selected, else false.
//	 * @param index
//	 * @return
//	 */
//	virtual bool isSelectedIndex(int index);
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns true if nothing is selected, else false.
//	 * @return
//	 */
//	virtual bool isSelectionEmpty();
//	//-------------------------------------------------------------------------
//	/**
//	 * Returns the cell index closest to the given location in the list's
//	 * coordinate system.
//	 * @param location
//	 * @return
//	 */
//	virtual int locationToIndex(const Point2D & location);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets the selection to be the set difference of the specified interval
//	 * and the current selection.
//	 * @param index0
//	 * @param index1
//	 */
//	virtual void removeSelectionInterval(int index0, int index1);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets the delegate that is used to paint each cell in the list.
//	 */
//	//void setCellRenderer(ListCellRenderer<? super E> cellRenderer);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets a fixed value to be used for the height of every cell in the list.
//	 * @param height
//	 */
//	virtual void setFixedCellHeight(const Coordinate & height);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets a fixed value to be used for the width of every cell in the list.
//	 * @param width
//	 */
//	virtual void setFixedCellWidth(const Coordinate & width);
//	//-------------------------------------------------------------------------
//	/**
//	 * 	Defines the way list cells are layed out.
//	 * @param layoutOrientation
//	 */
//	virtual void setLayoutOrientation(int layoutOrientation);
//	//-------------------------------------------------------------------------
//	/**
//	 *
//	 */
//	//TODO: void	setListData(E[] listData);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets the prototypeCellValue property, and then (if the new value
//	 * is non-null), computes the fixedCellWidth and fixedCellHeight properties
//	 * by requesting the cell renderer component for the given value (and
//	 * index 0) from the cell renderer, and using that component's preferred size.
//	 * @param prototypeCellValue
//	 */
//	virtual void setPrototypeCellValue(const T &prototypeCellValue);
//	//-------------------------------------------------------------------------
//	/**
//	 * Selects a single cell.
//	 * @param index
//	 */
//	virtual void setSelectedIndex(int index);
//	//-------------------------------------------------------------------------
//	/**
//	 * Changes the selection to be the set of indices specified by
//	 * the given container
//	 */
//	template<class IndexContainer>
//	void setSelectedIndices(const IndexContainer &indices);
//	//-------------------------------------------------------------------------
//	/**
//	 * Selects the specified object from the list.
//	 * @param anObject
//	 * @param shouldScroll
//	 */
//	virtual void setSelectedValue(const T &anObject, bool shouldScroll);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets the color used to draw the background of selected items,
//	 * which cell renderers can use fill selected cells.
//	 * @param selectionBackground
//	 */
//	virtual void setSelectionBackground(const ColorRGBA & selectionBackground);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets the color used to draw the foreground of selected items, which
//	 * cell renderers can use to render text and graphics.
//	 * @param selectionForeground
//	 */
//	virtual void setSelectionForeground(const ColorRGBA & selectionForeground);
//	//-------------------------------------------------------------------------
//	/**
//	 * Selects the specified interval.
//	 * @param anchor
//	 * @param lead
//	 */
//	virtual void setSelectionInterval(int anchor, int lead);
//	//-------------------------------------------------------------------------
//	/**
//	 * Sets the selection mode for the list.
//	 * @param selectionMode
//	 */
//	virtual void setSelectionMode(int selectionMode);
//	//-------------------------------------------------------------------------
//	/**
//	 * 	Sets the visibleRowCount property, which has different meanings
//	 * 	depending on the layout orientation: For a VERTICAL layout orientation,
//	 * 	this sets the preferred number of rows to display without requiring
//	 * 	scrolling; for other orientations, it affects the wrapping of cells.
//	 * @param visibleRowCount
//	 */
//	virtual void setVisibleRowCount(int visibleRowCount);

}; // List
}}} // namespace(s)

#endif /* SAMBAG_LIST_H */
