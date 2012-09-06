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
#include <sambag/com/Common.hpp>
#include "DefaultListCellRenderer.hpp"
#include <boost/foreach.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
 * @class List.
 */
template <
	class T,
	template <class> class _ListCellRenderer = DefaultListCellRenderer,
	template <class> class _ListModel = DefaultListModel,
	class _ListSelectionModel = DefaultListSelectionModel
>
class List: public AContainer,
		public _ListModel<T>,
		public _ListSelectionModel,
		public IScrollable
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _ListCellRenderer<T> ListCellRenderer;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ListCellRenderer> ListCellRendererPtr;
	//-------------------------------------------------------------------------
	typedef _ListModel<T> ListModel;
	//-------------------------------------------------------------------------
	typedef _ListSelectionModel ListSelectionModel;
	//-------------------------------------------------------------------------
	typedef List<T, _ListCellRenderer, _ListModel, _ListSelectionModel> Class;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<List> Ptr;
	//-------------------------------------------------------------------------
	enum Orientation {
		/**
		 * Indicates a vertical layout of cells, in a single column;
		 * the default layout.
		 * @see #setLayoutOrientation
		 */
		VERTICAL,
		/**
		 * Indicates a "newspaper style" layout with cells flowing vertically
		 * then horizontally.
		 * @see #setLayoutOrientation
		 */
		VERTICAL_WRAP,
		/**
		 * Indicates a "newspaper style" layout with cells flowing horizontally
		 * then vertically.
		 * @see #setLayoutOrientation
		 */
		HORIZONTAL_WRAP
	};
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_VISIBLEROWCOUNT;
	static const std::string PROPERTY_SELECTIONFOREGROUND;
	static const std::string PROPERTY_SELECTIONBACKGROUND;
	static const std::string PROPERTY_LAYOUTORIENTATION;
	static const std::string PROPERTY_FIXEDCELLWIDTH;
	static const std::string PROPERTY_FIXEDCELLHEIGHT;
	static const std::string PROPERTY_CELLRENDERER;
private:
	//-------------------------------------------------------------------------
	Coordinate fixedCellWidth;
	Coordinate fixedCellHeight;
	int horizontalScrollIncrement;
	T prototypeCellValue;
	int visibleRowCount;
	ColorRGBA selectionForeground;
	ColorRGBA selectionBackground;
	bool dragEnabled;
	Orientation layoutOrientation;
	ListCellRendererPtr cellRenderer;
public:
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const
	{
		return laf->getUI<List> ();
	}
protected:
	//-------------------------------------------------------------------------
	List() :
		fixedCellWidth(-1),
		fixedCellHeight(-1),
		horizontalScrollIncrement(-1),
		visibleRowCount(8),
		dragEnabled(false),
		layoutOrientation(VERTICAL)
	{
		setName("List");
		setOpaque(true);
	}
private:
	//-------------------------------------------------------------------------
	virtual void updateFixedCellSize();
	//-------------------------------------------------------------------------
	virtual void checkScrollableParameters(const Rectangle &visibleRect,
		int orientation);
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(List)
	//-------------------------------------------------------------------------
	/**
	 * Clears the selection; after calling this method, isSelectionEmpty
	 *  will return true.
	 */
	void clearSelection();
protected:
	//-------------------------------------------------------------------------
	std::string paramString();
public:
	//-------------------------------------------------------------------------
	/**
	 * Scrolls the list within an enclosing viewport to make the
	 * specified cell completely visible.
	 * @param index
	 */
	virtual void ensureIndexIsVisible(int index);
	//-------------------------------------------------------------------------
	/**
	 * Returns the bounding rectangle, in the list's coordinate system,
	 * for the range of cells specified by the two indices.
	 * @param index0
	 * @param index1
	 * @return
	 */
	virtual Rectangle getCellBounds(int index0, int index1) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the object responsible for painting list items.
	 */
	ListCellRendererPtr	getCellRenderer() const {
		return cellRenderer;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the smallest list index that is currently visible.
	 * @return
	 */
	virtual int getFirstVisibleIndex() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the value of the fixedCellHeight property.
	 * @return
	 */
	virtual Coordinate getFixedCellHeight() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the value of the fixedCellWidth property.
	 * @return
	 */
	virtual Coordinate getFixedCellWidth() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the largest list index that is currently visible.
	 * @return
	 */
	virtual int getLastVisibleIndex() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the layout orientation property for the list: VERTICAL if the
	 * layout is a single column of cells, VERTICAL_WRAP if the layout is
	 * "newspaper style" with the content flowing vertically then horizontally,
	 *  or HORIZONTAL_WRAP if the layout is "newspaper style" with the content
	 *   flowing horizontally then vertically.
	 * @return
	 */
	virtual Orientation getLayoutOrientation() const;
	//-------------------------------------------------------------------------
	/**
	 * 	Returns the lead selection index.
	 * @return
	 */
	virtual int getLeadSelectionIndex() const;
	//-------------------------------------------------------------------------
	/**
	 * Computes the size of viewport needed to display visibleRowCount rows.
	 * @return
	 */
	virtual Dimension getPreferredScrollableViewportSize() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the "prototypical" cell value -- a value used to calculate a
	 * fixed width and height for cells.
	 * @return
	 */
	virtual const T & getPrototypeCellValue() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the distance to scroll to expose the next or previous block.
	 * @param visibleRect
	 * @param orientation
	 * @param direction
	 * @return
	 */
	virtual Coordinate
	getScrollableBlockIncrement(const Rectangle &visibleRect,
		int orientation, int direction) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if this JList is displayed in a JViewport and the viewport
	 * is taller than the list's preferred height, or if the layout orientation
	 * is VERTICAL_WRAP and visibleRowCount <= 0; otherwise returns false.
	 * @return
	 */
	virtual bool getScrollableTracksViewportHeight() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if this JList is displayed in a JViewport and the viewport
	 * is wider than the list's preferred width, or if the layout orientation is
	 * HORIZONTAL_WRAP and visibleRowCount <= 0; otherwise returns false.
	 * @return
	 */
	virtual bool getScrollableTracksViewportWidth() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the distance to scroll to expose the next or previous row
	 * (for vertical scrolling) or column (for horizontal scrolling).
	 * @param visibleRect
	 * @param orientation
	 * @param direction
	 * @return
	 */
	virtual Coordinate
	getScrollableUnitIncrement(const Rectangle &visibleRect,
		int orientation, int direction) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the smallest selected cell index; the selection when only a
	 * single item is selected in the list.
	 * @return
	 */
	virtual int getSelectedIndex() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns an array of all of the selected indices, in increasing order.
	 */
	template<class IndexContainer>
	void getSelectedIndices(IndexContainer &out) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the value for the smallest selected cell index; the selected
	 * value when only a single item is selected in the list.
	 * @return
	 */
	virtual const T * getSelectedValue() const;
	//-------------------------------------------------------------------------
	/**
	 *
	 */
	template<class Container>
	void getSelectedValues(Container &out) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the color used to draw the background of selected items.
	 * @return
	 */
	virtual ColorRGBA getSelectionBackground() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the color used to draw the foreground of selected items.
	 * @return
	 */
	virtual ColorRGBA getSelectionForeground() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the value of the visibleRowCount property.
	 * @return
	 */
	int getVisibleRowCount() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the origin of the specified item in the list's coordinate system.
	 * @param index
	 * @return
	 */
	virtual Point2D indexToLocation(int index) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if the specified index is selected, else false.
	 * @param index
	 * @return
	 */
	virtual bool isSelectedIndex(int index) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if nothing is selected, else false.
	 * @return
	 */
	virtual bool isSelectionEmpty() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the cell index closest to the given location in the list's
	 * coordinate system.
	 * @param location
	 * @return
	 */
	virtual int locationToIndex(const Point2D & location) const;
	//-------------------------------------------------------------------------
	/**
	 * Sets the delegate that is used to paint each cell in the list.
	 */
	void setCellRenderer(ListCellRendererPtr cellRenderer);
	//-------------------------------------------------------------------------
	/**
	 * Sets a fixed value to be used for the height of every cell in the list.
	 * @param height
	 */
	virtual void setFixedCellHeight(const Coordinate & height);
	//-------------------------------------------------------------------------
	/**
	 * Sets a fixed value to be used for the width of every cell in the list.
	 * @param width
	 */
	virtual void setFixedCellWidth(const Coordinate & width);
	//-------------------------------------------------------------------------
	/**
	 * Defines the way list cells are layed out. Consider a {@code JList}
	 * with five cells. Cells can be layed out in one of the following ways:
	 * <p>
	 * <pre>
	 * VERTICAL:          0
	 *                    1
	 *                    2
	 *                    3
	 *                    4
	 *
	 * HORIZONTAL_WRAP:   0  1  2
	 *                    3  4
	 *
	 * VERTICAL_WRAP:     0  3
	 *                    1  4
	 *                    2
	 * @param layoutOrientation
	 */
	virtual void setLayoutOrientation(Orientation layoutOrientation);
	//-------------------------------------------------------------------------
	/**
	 * Sets the prototypeCellValue property, and then (if the new value
	 * is non-null), computes the fixedCellWidth and fixedCellHeight properties
	 * by requesting the cell renderer component for the given value (and
	 * index 0) from the cell renderer, and using that component's preferred size.
	 * @param prototypeCellValue
	 */
	virtual void setPrototypeCellValue(const T &prototypeCellValue);
	//-------------------------------------------------------------------------
	/**
	 * Selects a single cell.
	 * @param index
	 */
	virtual void setSelectedIndex(int index);
	//-------------------------------------------------------------------------
	/**
	 * Changes the selection to be the set of indices specified by
	 * the given container
	 */
	template<class IndexContainer>
	void setSelectedIndices(const IndexContainer &indices);
	//-------------------------------------------------------------------------
	/**
	 * Selects the specified object from the list.
	 * @param anObject
	 * @param shouldScroll
	 */
	virtual void setSelectedValue(const T &anObject, bool shouldScroll);
	//-------------------------------------------------------------------------
	/**
	 * Sets the color used to draw the background of selected items,
	 * which cell renderers can use fill selected cells.
	 * @param selectionBackground
	 */
	virtual void setSelectionBackground(const ColorRGBA & selectionBackground);
	//-------------------------------------------------------------------------
	/**
	 * Sets the color used to draw the foreground of selected items, which
	 * cell renderers can use to render text and graphics.
	 * @param selectionForeground
	 */
	virtual void setSelectionForeground(const ColorRGBA & selectionForeground);
	//-------------------------------------------------------------------------
	/**
	 * 	Sets the visibleRowCount property, which has different meanings
	 * 	depending on the layout orientation: For a VERTICAL layout orientation,
	 * 	this sets the preferred number of rows to display without requiring
	 * 	scrolling; for other orientations, it affects the wrapping of cells.
	 * @param visibleRowCount
	 */
	virtual void setVisibleRowCount(int visibleRowCount);
}; // List
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_VISIBLEROWCOUNT =
		"visible_rowcount";
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_SELECTIONFOREGROUND =
		"selection_foreground";
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_SELECTIONBACKGROUND =
		"selection_background";
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_LAYOUTORIENTATION =
		"property_layoutorientation";
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_FIXEDCELLWIDTH =
		"fixed_cellwitdh";
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_FIXEDCELLHEIGHT =
		"fixed_cellheight";
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const std::string List<T, CR, DM, SM>::PROPERTY_CELLRENDERER =
		"cellrender";
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::updateFixedCellSize() {
	ListCellRendererPtr cr = getCellRenderer();
	const T &value = getPrototypeCellValue();

	if (cr) {
		AComponentPtr c = cr->getListCellRendererComponent(
				getPtr(), value, 0, false, false);

		/* The ListUI implementation will add Component c to its private
		 * CellRendererPane however we can't assume that's already
		 * been done here.  So we temporarily set the one "inherited"
		 * property that may affect the renderer components preferred size:
		 * its font.
		 */
		Font f = c->getFont();
		c->setFont(getFont());

		Dimension d = c->getPreferredSize();
		fixedCellWidth = d.width();
		fixedCellHeight = d.height();

		c->setFont(f);
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::checkScrollableParameters(
	const Rectangle &visibleRect, int orientation)
{

}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::ensureIndexIsVisible(int index) {
	Rectangle cellBounds = getCellBounds(index, index);
	if (cellBounds != NULL_RECTANGLE) {
		// TODO: scrollRectToVisible(cellBounds);
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Rectangle List<T, CR, DM, SM>::getCellBounds(int index0, int index1) const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_RECTANGLE;
	/*
	 * ListUI ui = getUI();
	 * return (ui != null) ? ui.getCellBounds(this, index0, index1) : null;
	 */
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::setCellRenderer(
	List<T, CR, DM, SM>::ListCellRendererPtr cellRenderer)
{
	ListCellRendererPtr oldValue = this->cellRenderer;
	this->cellRenderer = cellRenderer;

	/* If the cellRenderer has changed and prototypeCellValue
	 * was set, then recompute fixedCellWidth and fixedCellHeight.
	 */
	if (cellRenderer && cellRenderer!=oldValue) {
		updateFixedCellSize();
	}

	firePropertyChanged(PROPERTY_CELLRENDERER, oldValue, cellRenderer);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int List<T, CR, DM, SM>::getFirstVisibleIndex() const {
	Rectangle r = getVisibleRect();
	int first = locationToIndex(r.x0());
	if (first != -1) {
		Rectangle bounds = getCellBounds(first, first);
		if (bounds != NULL_RECTANGLE) {
			//SwingUtilities.computeIntersection(r.x, r.y, r.width, r.height,
			//		bounds);
			boost::geometry::intersection<Rectangle::Base,
			Rectangle::Base, Rectangle::Base>(r, bounds, bounds);
			if (bounds.width() == 0 || bounds.height() == 0) {
				first = -1;
			}
		}
	}
	return first;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate List<T, CR, DM, SM>::getFixedCellHeight() const {
	return fixedCellHeight;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate List<T, CR, DM, SM>::getFixedCellWidth() const {
	return fixedCellWidth;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int List<T, CR, DM, SM>::getLastVisibleIndex() const {
	Rectangle r = getVisibleRect();
	Point2D lastPoint;
	lastPoint =
		Point2D((r.x0().x() + r.width()) - 1., (r.x0().y() + r.height()) - 1.);

	int location = locationToIndex(lastPoint);

	if (location != -1) {
		Rectangle bounds = getCellBounds(location, location);

		if (bounds != NULL_RECTANGLE) {
			//SwingUtilities.computeIntersection(r.x, r.y, r.width, r.height,
			//		bounds);
			boost::geometry::intersection<Rectangle::Base,
			Rectangle::Base, Rectangle::Base>(r, bounds, bounds);
			if (bounds.width() == 0 || bounds.height() == 0) {
				// Try the top left(LTR) or top right(RTL) corner, and
				// then go across checking each cell for HORIZONTAL_WRAP.
				// Try the lower left corner, and then go across checking
				// each cell for other list layout orientation.
				bool isHorizontalWrap = (getLayoutOrientation()
						== HORIZONTAL_WRAP);
				Point2D visibleLocation = isHorizontalWrap ? Point2D(
					lastPoint.x(), r.x0().y()) : Point2D(r.x0().x(), lastPoint.y());
				int last;
				int visIndex = -1;
				int lIndex = location;
				location = -1;

				do {
					last = visIndex;
					visIndex = locationToIndex(visibleLocation);

					if (visIndex != -1) {
						bounds = getCellBounds(visIndex, visIndex);
						if (visIndex != lIndex && bounds != NULL_RECTANGLE
								&& bounds.contains(visibleLocation)) {
							location = visIndex;
							if (isHorizontalWrap) {
								visibleLocation.y(bounds.x0().y() + bounds.height());
								if (visibleLocation.y() >= lastPoint.y()) {
									// Past visible region, bail.
									last = visIndex;
								}
							} else {
								visibleLocation.x(bounds.x0().x() + bounds.width());
								if (visibleLocation.x() >= lastPoint.x()) {
									// Past visible region, bail.
									last = visIndex;
								}
							}

						} else {
							last = visIndex;
						}
					}
				} while (visIndex != -1 && last != visIndex);
			}
		}
	}
	return location;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
typename List<T, CR, DM, SM>::Orientation
List<T, CR, DM, SM>::getLayoutOrientation() const
{
	return layoutOrientation;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int List<T, CR, DM, SM>::getLeadSelectionIndex() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return -1;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Dimension List<T, CR, DM, SM>::getPreferredScrollableViewportSize() const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_DIMENSION;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const T & List<T, CR, DM, SM>::getPrototypeCellValue() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return prototypeCellValue;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate List<T, CR, DM, SM>::
getScrollableBlockIncrement(const Rectangle &visibleRect, int orientation,
		int direction) const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_NUMBER;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
bool List<T, CR, DM, SM>::getScrollableTracksViewportHeight() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
bool List<T, CR, DM, SM>::getScrollableTracksViewportWidth() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate List<T, CR, DM, SM>::
getScrollableUnitIncrement(const Rectangle &visibleRect, int orientation,
		int direction) const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_NUMBER;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int List<T, CR, DM, SM>::getSelectedIndex() const {
	return ListSelectionModel::getMinSelectionIndex();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
template <class IndexContainer>
void List<T, CR, DM, SM>::getSelectedIndices(IndexContainer &out) const
{
	int iMin = ListSelectionModel::getMinSelectionIndex();
	int iMax = ListSelectionModel::getMaxSelectionIndex();

	if ((iMin < 0) || (iMax < 0)) {
		return;
	}
	int n = 0;
	for (int i = iMin; i <= iMax; ++i) {
		if (isSelectedIndex(i)) {
			out.push_back(i);
		}
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const T * List<T, CR, DM, SM>::getSelectedValue() const {
	int i = ListSelectionModel::getMinSelectionIndex();
	return (i == -1) ? NULL : &(ListModel::get(i));
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
ColorRGBA List<T, CR, DM, SM>::getSelectionBackground() const {
	return selectionBackground;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
ColorRGBA List<T, CR, DM, SM>::getSelectionForeground() const {
	return selectionForeground;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int List<T, CR, DM, SM>::getVisibleRowCount() const {
	return visibleRowCount;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Point2D List<T, CR, DM, SM>::indexToLocation(int index) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_POINT2D;
	/*
	 * ListUI ui = getUI();
	 * return (ui != null) ? ui.indexToLocation(this, index) : null;
	 */
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
bool List<T, CR, DM, SM>::isSelectedIndex(int index) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
bool List<T, CR, DM, SM>::isSelectionEmpty() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int List<T, CR, DM, SM>::locationToIndex(const Point2D & location) const {
	/*ListUI ui = getUI();
	return (ui != null) ? ui.locationToIndex(this, location) : -1;*/
	SAMBA_LOG_NOT_YET_IMPL();
	return -1;
}
//-----------------------------------------------------------------------------
//void setCellRenderer(ListCellRenderer<? super E> cellRenderer);
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::setFixedCellHeight(const Coordinate & height) {
	Coordinate oldValue = fixedCellHeight;
	fixedCellHeight = height;
	firePropertyChanged(PROPERTY_FIXEDCELLHEIGHT, oldValue, fixedCellHeight);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::setFixedCellWidth(const Coordinate & width) {
	Coordinate oldValue = fixedCellWidth;
	fixedCellWidth = width;
	firePropertyChanged(PROPERTY_FIXEDCELLWIDTH, oldValue, fixedCellWidth);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::setLayoutOrientation(
		List<T, CR, DM, SM>::Orientation layoutOrientation)
{
	Orientation oldValue = this->layoutOrientation;
	this->layoutOrientation = layoutOrientation;
	firePropertyChanged(PROPERTY_LAYOUTORIENTATION,oldValue,layoutOrientation);

}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
template<class Container>
void List<T, CR, DM, SM>::getSelectedValues(Container &out) const {

	int iMin = ListSelectionModel::getMinSelectionIndex();
	int iMax = ListSelectionModel::getMaxSelectionIndex();

	if ((iMin < 0) || (iMax < 0)) {
		return;
	}

	int n = 0;
	for (int i = iMin; i <= iMax; i++) {
		if (ListSelectionModel::isSelectedIndex(i)) {
			ListModel::get[i];
		}
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::
setPrototypeCellValue(const T &prototypeCellValue)
{
	T oldValue = this->prototypeCellValue;
	this->prototypeCellValue = prototypeCellValue;

	/* If the prototypeCellValue has changed and is non-null,
	 * then recompute fixedCellWidth and fixedCellHeight.
	 */

	if (prototypeCellValue!=oldValue) {
		updateFixedCellSize();
	}

	firePropertyChanged("prototypeCellValue", oldValue, prototypeCellValue);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::
setSelectedIndex(int index)
{
	if (index >= (int)ListModel::getSize()) {
		return;
	}
	ListSelectionModel::setSelectionInterval(index, index);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
template<class IndexContainer>
void List<T, CR, DM, SM>::
setSelectedIndices(const IndexContainer &indices)
{
	ListSelectionModel::clearSelection();
	int size = (int)ListModel::getSize();
	BOOST_FOREACH(int i, indices) {
		if (i < size) {
			ListSelectionModel::addSelectionInterval(i, i);
		}
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
//TODO: was here
void List<T, CR, DM, SM>::
setSelectedValue(const T &anObject, bool shouldScroll)
{
	if (&anObject == getSelectedValue())
		return;

	int i, c;
	for (i = 0, c = ListModel::getSize(); i < c; i++) {
		if (&anObject == &(ListModel::get(i))) {
			setSelectedIndex(i);
			if (shouldScroll)
				ensureIndexIsVisible(i);
			redraw();
			return;
		}
	}
	setSelectedIndex(-1);

	redraw();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::
setSelectionBackground(const ColorRGBA & selectionBackground)
{
	ColorRGBA oldValue = this->selectionBackground;
	this->selectionBackground = selectionBackground;
	firePropertyChanged(PROPERTY_SELECTIONBACKGROUND,
			oldValue, selectionBackground);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::
setSelectionForeground(const ColorRGBA & selectionForeground)
{
	ColorRGBA oldValue = this->selectionForeground;
	this->selectionForeground = selectionForeground;
	firePropertyChanged(PROPERTY_SELECTIONFOREGROUND,
			oldValue, selectionForeground);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void List<T, CR, DM, SM>::setVisibleRowCount(int visibleRowCount) {
	int oldValue = this->visibleRowCount;
	this->visibleRowCount = std::max(0, visibleRowCount);
	firePropertyChanged(PROPERTY_VISIBLEROWCOUNT, oldValue, visibleRowCount);
}
}}} // namespace(s)

#endif /* SAMBAG_LIST_H */
