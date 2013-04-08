/*
 * AList.hpp
 *
 *  Created on: Thu Jul  5 16:54:31 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ALIST_H
#define SAMBAG_ALIST_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include "ui/ALookAndFeel.hpp"
#include "ui/UIManager.hpp"
#include "IScrollable.hpp"
#include "AScrollbar.hpp"
#include <sambag/com/Common.hpp>
#include <boost/foreach.hpp>
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include "Viewport.hpp"
#include "ui/IListUI.hpp"
#include "events/ListDataEvent.hpp"

namespace sambag { namespace disco { namespace components {
struct ListConstants {
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
	static const std::string PROPERTY_PROTOTYPECELLVALUE;
};
//=============================================================================
/** 
 * @class AList.
 */
template <
	class T,
	template <class> class _ListCellRenderer,
	template <class> class _ListModel,
	class _ListSelectionModel
>
class AList : public AContainer,
		public _ListModel<T>,
		public _ListSelectionModel,
		public IScrollable,
		public ListConstants
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef T ValueType;
	//-------------------------------------------------------------------------
	typedef _ListCellRenderer<T> ListCellRenderer;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ListCellRenderer> ListCellRendererPtr;
	//-------------------------------------------------------------------------
	typedef _ListModel<T> ListModel;
	//-------------------------------------------------------------------------
	typedef _ListSelectionModel ListSelectionModel;
	//-------------------------------------------------------------------------
	typedef AList<T, _ListCellRenderer, _ListModel, _ListSelectionModel> Class;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AList> Ptr;
private:
	//-------------------------------------------------------------------------
	Coordinate fixedCellWidth;
	Coordinate fixedCellHeight;
	int horizontalScrollIncrement;
	T prototypeCellValue;
	int visibleRowCount;
	IPattern::Ptr selectionForeground;
	IPattern::Ptr selectionBackground;
	bool dragEnabled;
	Orientation layoutOrientation;
	ListCellRendererPtr cellRenderer;
public:
protected:
	//-------------------------------------------------------------------------
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	AList() :
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
		int orientation) const;
public:
protected:
	//-------------------------------------------------------------------------
	/**
	 * @override
	 * @param laf
	 */
	virtual void installLookAndFeel(ui::ALookAndFeelPtr laf);
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<Class>(Super::getPtr());
	}
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
	virtual Dimension getPreferredScrollableViewportSize();
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
		ScrollbarConstants::Orientation orientation,
		ScrollbarConstants::Direction direction) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if this JList is displayed in a JViewport and the viewport
	 * is taller than the list's preferred height, or if the layout orientation
	 * is VERTICAL_WRAP and visibleRowCount <= 0; otherwise returns false.
	 * @return
	 */
	virtual bool getScrollableTracksViewportHeight();
	//-------------------------------------------------------------------------
	/**
	 * Returns true if this JList is displayed in a JViewport and the viewport
	 * is wider than the list's preferred width, or if the layout orientation is
	 * HORIZONTAL_WRAP and visibleRowCount <= 0; otherwise returns false.
	 * @return
	 */
	virtual bool getScrollableTracksViewportWidth();
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
		ScrollbarConstants::Orientation orientation,
		ScrollbarConstants::Direction direction) const;
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
	virtual IPattern::Ptr getSelectionBackground() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the color used to draw the foreground of selected items.
	 * @return
	 */
	virtual IPattern::Ptr getSelectionForeground() const;
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
	virtual void setSelectionBackground(IPattern::Ptr selectionBackground);
	//-------------------------------------------------------------------------
	/**
	 * Sets the color used to draw the foreground of selected items, which
	 * cell renderers can use to render text and graphics.
	 * @param selectionForeground
	 */
	virtual void setSelectionForeground(IPattern::Ptr selectionForeground);
	//-------------------------------------------------------------------------
	/**
	 * 	Sets the visibleRowCount property, which has different meanings
	 * 	depending on the layout orientation: For a VERTICAL layout orientation,
	 * 	this sets the preferred number of rows to display without requiring
	 * 	scrolling; for other orientations, it affects the wrapping of cells.
	 * @param visibleRowCount
	 */
	virtual void setVisibleRowCount(int visibleRowCount);
}; // AList
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::installLookAndFeel(ui::ALookAndFeelPtr laf) {
	Super::installLookAndFeel(laf);
	if (cellRenderer) {
		cellRenderer->installLookAndFeel(laf);
		Coordinate oldW = fixedCellWidth;
		Coordinate oldH = fixedCellHeight;
		updateFixedCellSize();
		// restore previous values if set
		if (oldW > 0.)
			fixedCellWidth = oldW;
		if (oldH > 0.)
			fixedCellHeight = oldH;
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::postConstructor() {
	Super::postConstructor();
	setCellRenderer(ListCellRenderer::create());
}
//-----------------------------------------------------------------------------
namespace {
	SAMBAG_PROPERTY_TAG(TagMinListWidth, "List.minCellWidth");
	SAMBAG_PROPERTY_TAG(TagMinListHeight, "List.minCellHeight");
}
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::updateFixedCellSize() {
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

		Coordinate minCellWidth = ui::getUIPropertyCached<TagMinListWidth>(Coordinate(25.));
		Coordinate minCellHeight = ui::getUIPropertyCached<TagMinListHeight>(Coordinate(15.));

		Dimension d = c->getPreferredSize();
		fixedCellWidth = std::max( d.width(), minCellWidth);
		fixedCellHeight = std::max( d.height(), minCellHeight);

		c->setFont(f);
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::checkScrollableParameters (
	const Rectangle &visibleRect, int orientation) const
{
	if (visibleRect == NULL_RECTANGLE) {
		using namespace sambag::com::exceptions;
		SAMBAG_THROW(IllegalArgumentException,"visibleRect must be non-null");
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::ensureIndexIsVisible(int index) {
	Rectangle cellBounds = getCellBounds(index, index);
	if (cellBounds != NULL_RECTANGLE) {
		scrollRectToVisible(cellBounds);
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Rectangle AList<T, CR, DM, SM>::getCellBounds(int index0, int index1) const
{
	using namespace ui;
	IListUI::Ptr ui = boost::shared_dynamic_cast<IListUI>(getUI());
	return (ui) ? ui->getCellBounds(getPtr(), index0, index1) : NULL_RECTANGLE;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::setCellRenderer(
	typename AList<T, CR, DM, SM>::ListCellRendererPtr cellRenderer)
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
int AList<T, CR, DM, SM>::getFirstVisibleIndex() const {
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
Coordinate AList<T, CR, DM, SM>::getFixedCellHeight() const {
	return fixedCellHeight;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate AList<T, CR, DM, SM>::getFixedCellWidth() const {
	return fixedCellWidth;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int AList<T, CR, DM, SM>::getLastVisibleIndex() const {
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
typename AList<T, CR, DM, SM>::Orientation
AList<T, CR, DM, SM>::getLayoutOrientation() const
{
	return layoutOrientation;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int AList<T, CR, DM, SM>::getLeadSelectionIndex() const {
	return ListSelectionModel::getLeadSelectionIndex();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Dimension AList<T, CR, DM, SM>::getPreferredScrollableViewportSize() {
	if (getLayoutOrientation() != VERTICAL) {
		return getPreferredSize();
	}
	Insets insets = getInsets();
	Coordinate dx = insets.left() + insets.right();
	Coordinate dy = insets.top() + insets.bottom();

	int visibleRowCount = getVisibleRowCount();
	Coordinate fixedCellWidth = getFixedCellWidth();
	Coordinate fixedCellHeight = getFixedCellHeight();

	if ((fixedCellWidth > 0.) && (fixedCellHeight > 0.)) {
		Coordinate width = fixedCellWidth + dx;
		Coordinate height = (visibleRowCount * fixedCellHeight) + dy;
		return Dimension(width, height);
	} else if (ListModel::getSize() > 0) {
		Coordinate width = getPreferredSize().width();
		Coordinate height;
		Rectangle r = getCellBounds(0, 0);
		if (r != NULL_RECTANGLE) {
			height = (visibleRowCount * r.height()) + dy;
		} else {
			// Will only happen if UI null, shouldn't matter what we return
			height = 1;
		}
		return Dimension(width, height);
	} else {
		fixedCellWidth = (fixedCellWidth > 0.) ?
				fixedCellWidth : Coordinate(256.);
		fixedCellHeight = (fixedCellHeight > 0.) ?
				fixedCellHeight : Coordinate(16.);
		return Dimension(fixedCellWidth, fixedCellHeight * visibleRowCount);
	}
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
const T & AList<T, CR, DM, SM>::getPrototypeCellValue() const {
	return prototypeCellValue;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate AList<T, CR, DM, SM>::
getScrollableBlockIncrement(const Rectangle &visibleRect,
		ScrollbarConstants::Orientation orientation,
		ScrollbarConstants::Direction direction) const
{
	checkScrollableParameters(visibleRect, orientation);
	if (orientation == ScrollbarConstants::VERTICAL) {
		Coordinate inc = visibleRect.height();
		/* Scroll Down */
		if (direction == ScrollbarConstants::INCR) {
			// last cell is the lowest left cell
			int last = locationToIndex(
					Point2D(visibleRect.x0().x(),
							visibleRect.x0().y() + visibleRect.height() - 1.));
			if (last != -1) {
				Rectangle lastRect = getCellBounds(last, last);
				if (lastRect != NULL_RECTANGLE) {
					inc = lastRect.x0().y() - visibleRect.x0().y();
					if ((inc == 0) && (last < ListModel::getSize() - 1.)) {
						inc = lastRect.height();
					}
				}
			}
		}
		/* Scroll Up */
		else {
			int newFirst =
					locationToIndex(
							Point2D(visibleRect.x0().x(),
									visibleRect.x0().y() -
									visibleRect.height())
							);
			int first = getFirstVisibleIndex();
			if (newFirst != -1) {
				if (first == -1) {
					first = locationToIndex(visibleRect.x0());
				}
				Rectangle newFirstRect = getCellBounds(newFirst, newFirst);
				Rectangle firstRect = getCellBounds(first, first);
				if ((newFirstRect != NULL_RECTANGLE) &&
					(firstRect != NULL_RECTANGLE))
				{
					while ((newFirstRect.x0().y()
							+ visibleRect.height() < firstRect.x0().y()
							+ firstRect.height()) && (newFirstRect.x0().y()
							< firstRect.x0().y()))
					{
						newFirst++;
						newFirstRect = getCellBounds(newFirst, newFirst);
					}
					inc = visibleRect.x0().y() - newFirstRect.x0().y();
					if ((inc <= 0.) && (newFirstRect.x0().y() > 0.)) {
						newFirst--;
						newFirstRect = getCellBounds(newFirst, newFirst);
						if (newFirstRect != NULL_RECTANGLE) {
							inc = visibleRect.x0().y() - newFirstRect.x0().y();
						}
					}
				}
			}
		}
		return inc;
	} else if (orientation == ScrollbarConstants::HORIZONTAL
			&& getLayoutOrientation() != VERTICAL) {
		Coordinate inc = visibleRect.width();
		if (direction == ScrollbarConstants::INCR) {
			// position is upper right if ltr, or upper left otherwise
			Coordinate x = visibleRect.x0().x() + visibleRect.width() - 1;
			int last = locationToIndex(Point2D(x, visibleRect.x0().y()));
			if (last != -1) {
				Rectangle lastRect = getCellBounds(last, last);
				if (lastRect != NULL_RECTANGLE) {
					inc = lastRect.x0().x() - visibleRect.x0().x();
					if (inc == ScrollbarConstants::DECR) {
						inc += lastRect.width();
					} else if ((inc == 0) &&
							(last < (int)ListModel::getSize() - 1)
					){
						inc = lastRect.width();
					}
				}
			}
		}
		else {
			// position is upper left corner of the visibleRect shifted
			// left by the visibleRect.width if ltr, or upper right shifted
			// right by the visibleRect.width otherwise
			Coordinate x = visibleRect.x0().x() - visibleRect.width();
			int first = locationToIndex(Point2D(x, visibleRect.x0().y()));
			if (first != -1) {
				Rectangle firstRect = getCellBounds(first, first);
				if (firstRect != NULL_RECTANGLE) {
					// the right of the first cell
					Coordinate firstRight = firstRect.x0().x() +
							firstRect.width();
				if ((firstRect.x0().x() < visibleRect.x0().x()
						- visibleRect.width())
						&& (firstRight < visibleRect.x0().x()))
					{
						inc = visibleRect.x0().x() - firstRight;
					} else {
						inc = visibleRect.x0().x() - firstRect.x0().x();
					}
				}
			}
		}
		return inc;
	}
	return visibleRect.width();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
bool AList<T, CR, DM, SM>::getScrollableTracksViewportHeight() {
	if (getLayoutOrientation() == VERTICAL_WRAP && getVisibleRowCount() <= 0) {
		return true;
	}
	Viewport::Ptr parent = // TODO:SwingUtilities.getUnwrappedParent(this);
			boost::shared_dynamic_cast<Viewport> (
				getParent()
			);
	if (parent) {
		return parent->getHeight() > getPreferredSize().height();
	}
	return false;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
bool AList<T, CR, DM, SM>::getScrollableTracksViewportWidth() {
	if (getLayoutOrientation() == HORIZONTAL_WRAP && getVisibleRowCount() <= 0)
	{
		return true;
	}
	Viewport::Ptr parent = // TODO:SwingUtilities.getUnwrappedParent(this);
			boost::shared_dynamic_cast<Viewport> (
				getParent()
			);

	if (parent) {
		return parent->getWidth() > getPreferredSize().width();
	}
	return false;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Coordinate AList<T, CR, DM, SM>::
getScrollableUnitIncrement(const Rectangle &visibleRect,
		ScrollbarConstants::Orientation orientation,
		ScrollbarConstants::Direction direction) const
{
	checkScrollableParameters(visibleRect, orientation);

	if (orientation == ScrollbarConstants::VERTICAL) {
		int row = locationToIndex(visibleRect.x0());

		if (row == -1) {
			return 0;
		} else {
			/* Scroll Down */
			if (direction == ScrollbarConstants::INCR) {
				Rectangle r = getCellBounds(row, row);
				return (r == NULL_RECTANGLE) ?
						0. : r.height() - (visibleRect.x0().y() - r.x0().y());
			}
			/* Scroll Up */
			else {
				Rectangle r = getCellBounds(row, row);

				/* The first row is completely visible and it's row 0.
				 * We're done.
				 */
				if ((r.x0().y() == visibleRect.x0().y()) && (row == 0)) {
					return 0.;
				}
				/* The first row is completely visible, return the
				 * height of the previous row or 0 if the first row
				 * is the top row of the list.
				 */
				else if (r.x0().y() == visibleRect.x0().y()) {
					Point2D loc = r.x0();
					loc.y( loc.y() - 1 );
					int prevIndex = locationToIndex(loc);
					Rectangle prevR = getCellBounds(prevIndex, prevIndex);
					if (prevR == NULL_RECTANGLE || prevR.x0().y() >= r.x0().y())
					{
						return 0;
					}
					return prevR.height();
				}
				/* The first row is partially visible, return the
				 * height of hidden part.
				 */
				else {
					return visibleRect.x0().y() - r.x0().y();
				}
			}
		}
	} else if (orientation == ScrollbarConstants::HORIZONTAL
			&& getLayoutOrientation() != VERTICAL) {
		int index;
		Point2D leadingPoint = visibleRect.x0();

		index = locationToIndex(leadingPoint);

		if (index != -1) {
			Rectangle cellBounds = getCellBounds(index, index);
			if (cellBounds != NULL_RECTANGLE && cellBounds.contains(leadingPoint))
			{
				Coordinate leadingVisibleEdge;
				Coordinate leadingCellEdge;
				leadingVisibleEdge = visibleRect.x0().x();
				leadingCellEdge = cellBounds.x0().x();

				if (leadingCellEdge != leadingVisibleEdge) {
					// scroll up
					if (direction == ScrollbarConstants::DECR) {
						// Show remainder of leading cell
						return std::abs(leadingVisibleEdge - leadingCellEdge);
					}
					// Hide rest of leading cell
					return leadingCellEdge + cellBounds.width()
							- leadingVisibleEdge;
				}
				// ASSUME: All cells are the same width
				return cellBounds.width();
			}
		}
	}
	Font f = getFont();
	return f.size;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int AList<T, CR, DM, SM>::getSelectedIndex() const {
	return ListSelectionModel::getMinSelectionIndex();
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
template <class IndexContainer>
void AList<T, CR, DM, SM>::getSelectedIndices(IndexContainer &out) const
{
	int iMin = ListSelectionModel::getMinSelectionIndex();
	int iMax = ListSelectionModel::getMaxSelectionIndex();

	if ((iMin < 0) || (iMax < 0)) {
		return;
	}
	int n = 0;
	for (int i = iMin; i <= iMax; ++i) {
		if (ListSelectionModel::isSelectedIndex(i)) {
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
const T * AList<T, CR, DM, SM>::getSelectedValue() const {
	int i = ListSelectionModel::getMinSelectionIndex();
	return (i == -1) ? NULL : &(ListModel::get(i));
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
IPattern::Ptr AList<T, CR, DM, SM>::getSelectionBackground() const {
	return selectionBackground;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
IPattern::Ptr AList<T, CR, DM, SM>::getSelectionForeground() const {
	return selectionForeground;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int AList<T, CR, DM, SM>::getVisibleRowCount() const {
	return visibleRowCount;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
Point2D AList<T, CR, DM, SM>::indexToLocation(int index) const {
	using namespace ui;
	IListUI::Ptr ui = boost::shared_dynamic_cast<IListUI>(getUI());
	return (ui) ? ui->indexToLocation(getPtr(), index) : NULL_POINT2D;
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
int AList<T, CR, DM, SM>::locationToIndex(const Point2D & location) const
{
	using namespace ui;
	IListUI::Ptr ui = boost::shared_dynamic_cast<IListUI>(getUI());
	return (ui) ? ui->locationToIndex(getPtr(), location) : -1;
}
//-----------------------------------------------------------------------------
//void setCellRenderer(ListCellRenderer<? super E> cellRenderer);
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::setFixedCellHeight(const Coordinate & height) {
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
void AList<T, CR, DM, SM>::setFixedCellWidth(const Coordinate & width) {
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
void AList<T, CR, DM, SM>::setLayoutOrientation(
	typename AList<T, CR, DM, SM>::Orientation layoutOrientation)
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
void AList<T, CR, DM, SM>::getSelectedValues(Container &out) const {

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
void AList<T, CR, DM, SM>::
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

	firePropertyChanged(PROPERTY_PROTOTYPECELLVALUE,
		oldValue, prototypeCellValue);
}
//-----------------------------------------------------------------------------
template < class T,
	template <class> class CR,
	template <class> class DM,
	class SM
>
void AList<T, CR, DM, SM>::
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
void AList<T, CR, DM, SM>::
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
void AList<T, CR, DM, SM>::
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
void AList<T, CR, DM, SM>::
setSelectionBackground(IPattern::Ptr selectionBackground)
{
	IPattern::Ptr oldValue = this->selectionBackground;
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
void AList<T, CR, DM, SM>::
setSelectionForeground(IPattern::Ptr selectionForeground)
{
	IPattern::Ptr oldValue = this->selectionForeground;
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
void AList<T, CR, DM, SM>::setVisibleRowCount(int visibleRowCount) {
	int oldValue = this->visibleRowCount;
	this->visibleRowCount = std::max(0, visibleRowCount);
	firePropertyChanged(PROPERTY_VISIBLEROWCOUNT, oldValue, visibleRowCount);
}
}}} // namespace(s)

#endif /* SAMBAG_ALIST_H */
