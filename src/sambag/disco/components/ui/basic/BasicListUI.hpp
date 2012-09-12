/*
 * BasicListUI.hpp
 *
 *  Created on: Wed Sep  5 12:04:40 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICLISTUI_H
#define SAMBAG_BASICLISTUI_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/disco/components/AList.hpp>
#include <sambag/disco/components/events/ListSelectionEvent.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/CellRendererPane.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/disco/components/Graphics.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/ui/IListUI.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicListUI.
  */
template <class _ListType>
class BasicListUI : public AComponentUI, public IListUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _ListType ListType;
	//-------------------------------------------------------------------------
	typedef BasicListUI<ListType> Class;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ListType> ListTypePtr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<ListType> ListTypeWPtr;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<Class> WPtr;
protected:
	//-------------------------------------------------------------------------
	BasicListUI() :
		updateLayoutStateNeeded(0), columnCount(0), rowsPerColumn(0) {}
	//-------------------------------------------------------------------------
	enum {
		modelChanged = 1 << 0,
		selectionModelChanged = 1 << 1,
		fontChanged = 1 << 2,
		fixedCellWidthChanged = 1 << 3,
		fixedCellHeightChanged = 1 << 4,
		prototypeCellValueChanged = 1 << 5,
		cellRendererChanged = 1 << 6,
		layoutOrientationChanged = 1 << 7,
		heightChanged = 1 << 8,
		widthChanged = 1 << 9,
		componentOrientationChanged = 1 << 10,
	};
	int updateLayoutStateNeeded;
	//-------------------------------------------------------------------------
	ListTypeWPtr _list;
	CellRendererPane::Ptr rendererPane;
	std::vector<Coordinate> cellHeights;
	Coordinate cellHeight;
	Coordinate cellWidth ;
	//-------------------------------------------------------------------------
	/**
	 * Height of the list. When asked to paint, if the current size of
	 * the list differs, this will update the layout state.
	 */
	Coordinate listHeight;
	//-------------------------------------------------------------------------
	/**
	 * Width of the list. When asked to paint, if the current size of
	 * the list differs, this will update the layout state.
	 */
	Coordinate listWidth;
	//-------------------------------------------------------------------------
	/**
	 * The layout orientation of the list.
	 */
	ListConstants::Orientation layoutOrientation;
	//-------------------------------------------------------------------------
	// Following ivars are used if the list is laying out horizontally
	/**
	 * Number of columns to create.
	 */
	int columnCount;
	//-------------------------------------------------------------------------
	/**
	 * Preferred height to make the list, this is only used if the
	 * the list is layed out horizontally.
	 */
	Coordinate preferredHeight;
	//-------------------------------------------------------------------------
	/**
	 * Number of rows per column. This is only used if the row height is
	 * fixed.
	 */
	int rowsPerColumn;
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	void installListeners(AComponentPtr c);
	//-------------------------------------------------------------------------
	void installDefaults(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Paint one List cell: compute the relevant state, get the "rubber stamp"
	 * cell renderer component, and then use the CellRendererPane to paint it.
	 * Subclasses may want to override this method rather than paint().
	 *
	 * @see #paint
	 */
	virtual void drawCell(IDrawContext::Ptr cn, int row,
		const Rectangle &rowBounds, ListTypePtr list, int leadIndex);
	//-------------------------------------------------------------------------
	virtual void drawImpl(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	void redrawList();
	//-------------------------------------------------------------------------
	void maybeUpdateLayoutState();
	//-------------------------------------------------------------------------
	void updateLayoutState();
	//-------------------------------------------------------------------------
	void updateHorizontalLayoutState(const Coordinate &fixedCellWidth,
			const Coordinate &fixedCellHeight);
	//-------------------------------------------------------------------------
	int getModelIndex(int column, int row) const;
	//-------------------------------------------------------------------------
	int getRowCount(int column) const;
	//-------------------------------------------------------------------------
	int convertModelToRow(int index) const;
	//-------------------------------------------------------------------------
	int convertModelToColumn(int index) const;
	//-------------------------------------------------------------------------
	int convertLocationToColumn(const Coordinate &x,
			const Coordinate & y) const;
	//-------------------------------------------------------------------------
	int convertLocationToRowInColumn(const Coordinate &y,
			int column) const;
	//-------------------------------------------------------------------------
	int convertLocationToRow(const Coordinate &x, const Coordinate &y0,
			bool closest) const;
	//-------------------------------------------------------------------------
	int convertLocationToModel(const Coordinate &x,
			const Coordinate &y) const;
	//-------------------------------------------------------------------------
	static int adjustIndex(int index, const ListType &list) {
		return index < (int)list.ListType::ListModel::getSize() ? index : -1;
	}
	//-------------------------------------------------------------------------
	Rectangle getCellBounds(ListType &list, int index1);
	//-------------------------------------------------------------------------
	Rectangle getCellBounds(ListType &list, int index1, int index2);
	//-------------------------------------------------------------------------
	Coordinate getHeight(int column, int row) const;
	//-------------------------------------------------------------------------
	Coordinate getRowHeight(int row) {
		return getHeight(0, row);
	}
	//-------------------------------------------------------------------------
	void onSelectionStateChanged(void *src,
			const events::ListSelectionEvent &ev);
public:
	//-------------------------------------------------------------------------
	virtual Rectangle getCellBounds(AComponentPtr c, int index1, int index2);
	//-------------------------------------------------------------------------
	virtual int locationToIndex(AComponentPtr c, const Point2D &p);
	//-------------------------------------------------------------------------
	virtual Point2D indexToLocation(AComponentPtr c, int index);
	//-------------------------------------------------------------------------
	ListTypePtr getListPtr() const { return _list.lock(); }
	//-------------------------------------------------------------------------
	/**
	 * Paint the rows that intersect the Graphics objects clipRect.  This
	 * method calls paintCell as necessary.  Subclasses
	 * may want to override these methods.
	 *
	 * @see #paintCell
	 */
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	Dimension getPreferredSize(AComponentPtr c);
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new Class());
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	virtual void installUI(AComponentPtr c);
public:
	///////////////////////////////////////////////////////////////////////////
	// Mouse Listener functions
	//-------------------------------------------------------------------------
	void onMouse(void *, const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mousePressed(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void adjustSelection(const events::MouseEvent &ev);
}; // BasicListUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::installUI(AComponentPtr c) {
	ListTypePtr list = boost::shared_dynamic_cast<ListType>(c);
	this->_list = list;
	layoutOrientation = list->getLayoutOrientation();
	rendererPane = CellRendererPane::create();
	list->add(rendererPane);
	columnCount = 1;
	cellHeight = cellWidth = -1.;
	listHeight = listWidth = -1.;
	updateLayoutStateNeeded = modelChanged;
	installDefaults(c);
	installListeners(c);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::installListeners(AComponentPtr c) {
	c->EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&Class::onMouse, this, _1, _2),
		getPtr()
	);
	ListTypePtr list = boost::shared_dynamic_cast<ListType>(c);
	SAMBAG_ASSERT(list);
	list->EventSender<events::ListSelectionEvent>::addTrackedEventListener(
		boost::bind(&Class::onSelectionStateChanged, this, _1, _2),
		getPtr()
	);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::installDefaults(AComponentPtr c) {
	UIManager &m = getUIManager();
	ColorRGBA col(0.7, 0.7, 0.7);
	m.getProperty("StringList.background", col);
	c->setBackground(col);

	col = ColorRGBA();
	m.getProperty("StringList.foreground", col);
	c->setForeground(col);
	ListTypePtr list = boost::shared_dynamic_cast<ListType>(c);
	SAMBAG_ASSERT(list);

	col = ColorRGBA(0.7, 0., 1.);
	m.getProperty("StringList.selectionBackground", col);
	list->setSelectionBackground(col);

	col = ColorRGBA(1., 1., 1.);
	m.getProperty("StringList.selectionForeground", col);
	list->setSelectionForeground(col);

}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::updateLayoutState() {

	ListTypePtr list = getListPtr();
	Coordinate fixedCellHeight = list->getFixedCellHeight();
	Coordinate fixedCellWidth = list->getFixedCellWidth();

	cellWidth = (fixedCellWidth != -1.) ? fixedCellWidth : (Coordinate)-1.;

	if (fixedCellHeight != -1.) {
		cellHeight = fixedCellHeight;
		cellHeights.clear();
	} else {
		cellHeight = -1;
		cellHeights.resize(list->ListType::ListModel::getSize());
	}

	if ((fixedCellWidth == -1.) || (fixedCellHeight == -1.)) {

		typename ListType::ListCellRendererPtr renderer =
				list->getCellRenderer();
		int dataModelSize = (int)list->ListType::ListModel::getSize();

		if (renderer) {
			for (int index = 0; index < dataModelSize; index++) {
				const typename ListType::ValueType &value =
						list->ListType::ListModel::get(index);
				AComponentPtr c = renderer->getListCellRendererComponent(list,
						value, index, false, false);
				rendererPane->add(c);
				Dimension cellSize = c->getPreferredSize();
				if (fixedCellWidth == -1.) {
					cellWidth = std::max(cellSize.width(), cellWidth);
				}
				if (fixedCellHeight == -1.) {
					cellHeights[index] = cellSize.height();
				}
			}
		} else {
			if (cellWidth == -1) {
				cellWidth = 0;
			}
			if (cellHeights.empty()) {
				cellHeights.resize(dataModelSize);
			}
			for (int index = 0; index < dataModelSize; index++) {
				cellHeights[index] = 0;
			}
		}
	}

	columnCount = 1;
	if (layoutOrientation != ListConstants::VERTICAL) {
		updateHorizontalLayoutState(fixedCellWidth, fixedCellHeight);
	}
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::drawCell(IDrawContext::Ptr cn, int row,
	const Rectangle &rowBounds, ListTypePtr list, int leadIndex)
{
	const typename ListType::ValueType &value = list->get(row);
	bool isSelected = list->ListType::ListSelectionModel::isSelectedIndex(row);
	bool cellHasFocus = row == leadIndex;
	typename ListType::ListCellRendererPtr cellRenderer =
			list->getCellRenderer();
	if (!cellRenderer)
		return;
	AComponentPtr rendererComponent =
			cellRenderer->getListCellRendererComponent(
					list, value, row, isSelected, cellHasFocus
			);

	rendererPane->drawComponent(cn, rendererComponent, list, rowBounds, true);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::draw(IDrawContext::Ptr cn, AComponentPtr c) {
	//Rectangle clip = cn->clipExtends();
	drawImpl(cn, c);
	//g.setClip(clip);
	//paintDropLine( g);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::drawImpl(IDrawContext::Ptr cn, AComponentPtr c) {
	namespace trans = boost::geometry::strategy::transform;
	ListTypePtr list = getListPtr();
	typedef trans::translate_transformer<Point2D, Point2D> Translate;
	switch (layoutOrientation) {
	case ListConstants::VERTICAL_WRAP:
		if (list->getHeight() != listHeight) {
			updateLayoutStateNeeded |= heightChanged;
			redrawList();
		}
		break;
	case ListConstants::HORIZONTAL_WRAP:
		if (list->getWidth() != listWidth) {
			updateLayoutStateNeeded |= widthChanged;
			redrawList();
		}
		break;
	default:
		break;
	}
	maybeUpdateLayoutState();

	typename ListType::ListCellRendererPtr renderer = list->getCellRenderer();
	int size;

	if ((!renderer) || (size = list->ListType::ListModel::getSize()) == 0) {
		return;
	}

	// Determine how many columns we need to paint
	Rectangle paintBounds = cn->clipExtends();

	int startColumn, endColumn;

	startColumn =
			convertLocationToColumn(paintBounds.x0().x(), paintBounds.x0().y());
	endColumn =
			convertLocationToColumn(paintBounds.x0().x() + paintBounds.width(),
				paintBounds.x0().y());
	Coordinate maxY = paintBounds.x0().y() + paintBounds.height();
	int leadIndex = adjustIndex(list->getLeadSelectionIndex(), *list);
	int rowIncrement =
		(layoutOrientation == ListConstants::HORIZONTAL_WRAP) ? columnCount : 1;

	for (int colCounter = startColumn; colCounter <= endColumn; colCounter++)
	{
		// And then how many rows in this columnn
		int row =
			convertLocationToRowInColumn(paintBounds.x0().y(), colCounter);
		int rowCount = getRowCount(colCounter);
		int index = getModelIndex(colCounter, row);
		Rectangle rowBounds = getCellBounds(*list, index, index);

		if (rowBounds == NULL_RECTANGLE) {
			// Not valid, bail!
			return;
		}
		while (row < rowCount && rowBounds.x0().y() < maxY && index < size) {
			ScratchGraphics g(cn);
			rowBounds.height( getHeight(colCounter, row) );
			g.setClip(rowBounds);
			g.clipRect(paintBounds);
			drawCell(cn, index, rowBounds, list, leadIndex);
			//rowBounds.y += rowBounds.height;
			Translate transl(0, rowBounds.height());
			boost::geometry::transform<Rectangle::Base, Rectangle::Base>
				(rowBounds, rowBounds, transl);
			index += rowIncrement;
			row++;
		}
	}
	// Empty out the renderer pane, allowing renderers to be gc'ed.
	//rendererPane->removeAll();
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::redrawList() {
	ListTypePtr list = getListPtr();
	list->revalidate();
	list->redraw();
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::maybeUpdateLayoutState() {
	if (updateLayoutStateNeeded != 0) {
		updateLayoutState();
		updateLayoutStateNeeded = 0;
	}
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::getModelIndex(int column, int row) const {
	ListTypePtr list = getListPtr();
	switch (layoutOrientation) {
	case ListConstants::VERTICAL_WRAP:
		return std::min((int)list->ListType::ListModel::getSize() - 1,
				rowsPerColumn * column + std::min(row, rowsPerColumn - 1));
	case ListConstants::HORIZONTAL_WRAP:
		return std::min((int)list->ListType::ListModel::getSize() - 1,
				row * columnCount + column);
	default:
		return row;
	}
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::getRowCount(int column) const {
	if (column < 0 || column >= columnCount) {
		return -1;
	}

	ListTypePtr list=getListPtr();

	if (layoutOrientation == ListConstants::VERTICAL
			|| (column == 0 && columnCount == 1)) {
		return (int)list->ListType::ListModel::getSize();
	}
	if (column >= columnCount) {
		return -1;
	}
	if (layoutOrientation == ListConstants::VERTICAL_WRAP) {
		if (column < (columnCount - 1)) {
			return rowsPerColumn;
		}
		return (int)list->ListType::ListModel::getSize()
			- (columnCount - 1) * rowsPerColumn;
	}
	int diff = columnCount - (columnCount * rowsPerColumn
			- (int)list->ListType::ListModel::getSize());

	if (column >= diff) {
		return std::max(0, rowsPerColumn - 1);
	}
	return rowsPerColumn;
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::convertLocationToColumn(const Coordinate &x,
		const Coordinate & y) const
{
	ListTypePtr list = getListPtr();
	if (cellWidth > 0.) {
		if (layoutOrientation == ListConstants::VERTICAL) {
			return 0;
		}
		Insets insets = list->getInsets();
		int col;
		col = (x - insets.left()) / cellWidth;
		if (col < 0.) {
			return 0;
		} else if (col >= columnCount) {
			return columnCount - 1;
		}
		return col;
	}
	return 0;
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::convertLocationToRowInColumn(const Coordinate &y,
		int column) const
{
	int x = 0;
	if (layoutOrientation != ListConstants::VERTICAL) {
		x = column * cellWidth;
	}
	return convertLocationToRow(x, y, true);
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::
locationToIndex(AComponentPtr c, const Point2D &p)
{
	maybeUpdateLayoutState();
	return convertLocationToModel(p.x(), p.y());
}
//-----------------------------------------------------------------------------
template <class LT>
Point2D BasicListUI<LT>::
indexToLocation(AComponentPtr c, int index)
{
	ListTypePtr list = getListPtr();
	maybeUpdateLayoutState();
	Rectangle rect = getCellBounds(list, index, index);

	if (rect != NULL_RECTANGLE) {
		return Point2D(rect.x0().x(), rect.x0().y());
	}
	return NULL_POINT2D;
}
//-----------------------------------------------------------------------------
template <class LT>
Rectangle BasicListUI<LT>::getCellBounds(AComponentPtr c,
		int index1, int index2)
{
	ListTypePtr list = boost::shared_dynamic_cast<ListType>(c);
	if (!list)
		return NULL_RECTANGLE;
	return getCellBounds(*(list.get()), index1, index2);
}
//-----------------------------------------------------------------------------
template <class LT>
Rectangle BasicListUI<LT>::getCellBounds(BasicListUI<LT>::ListType &list,
		int index1, int index2)
{
	maybeUpdateLayoutState();

	int minIndex = std::min(index1, index2);
	int maxIndex = std::max(index1, index2);

	if (minIndex >= (int)list.ListType::ListModel::getSize()) {
		return NULL_RECTANGLE;
	}

	Rectangle minBounds = getCellBounds(list, minIndex);

	if (minBounds == NULL_RECTANGLE) {
		return NULL_RECTANGLE;
	}
	if (minIndex == maxIndex) {
		return minBounds;
	}
	Rectangle maxBounds = getCellBounds(list, maxIndex);

	if (maxBounds != NULL_RECTANGLE) {
		if (layoutOrientation == ListConstants::HORIZONTAL_WRAP) {
			int minRow = convertModelToRow(minIndex);
			int maxRow = convertModelToRow(maxIndex);

			if (minRow != maxRow) {
				minBounds.x0().x(0.);
				minBounds.width(list.getWidth());
			}
		} else if (minBounds.x0().x() != maxBounds.x0().x()) {
			// Different columns
			minBounds.x0().y(0.);
			minBounds.height( list.getHeight() );
		}
		//minBounds.add(maxBounds);
		minBounds = union_(minBounds, maxBounds);
	}
	return minBounds;
}
//-----------------------------------------------------------------------------
template <class LT>
Rectangle BasicListUI<LT>::getCellBounds(BasicListUI<LT>::ListType &list,
		int index)
{
	maybeUpdateLayoutState();

	int row = convertModelToRow(index);
	int column = convertModelToColumn(index);

	if (row == -1 || column == -1) {
		return NULL_RECTANGLE;
	}

	Insets insets = list.getInsets();
	Coordinate x;
	Coordinate w = cellWidth;
	Coordinate y = insets.top();
	Coordinate h;
	switch (layoutOrientation) {
	case ListConstants::VERTICAL_WRAP:
	case ListConstants::HORIZONTAL_WRAP:
		x = insets.left() + column * cellWidth;
		y += cellHeight * (Coordinate)row;
		h = cellHeight;
		break;
	default:
		x = insets.left();
		if (cellHeights.empty()) {
			y += (cellHeight * (Coordinate)row);
		} else if (row >= (int)cellHeights.size()) {
			y = 0;
		} else {
			for (int i = 0; i < row; i++) {
				y += cellHeights[i];
			}
		}
		w = list.getWidth() - (insets.left() + insets.right());
		h = getRowHeight(index);
		break;
	}
	return Rectangle(x, y, w, h);
}
//-----------------------------------------------------------------------------
template <class LT>
Coordinate BasicListUI<LT>::getHeight(int column, int row) const {
	ListTypePtr list = getListPtr();
	if (column < 0 || column > columnCount || row < 0) {
		return -1.;
	}
	if (layoutOrientation != ListConstants::VERTICAL) {
		return cellHeight;
	}
	if (row >= (int)list->ListType::ListModel::getSize()) {
		return -1;
	}
	return (cellHeights.empty()) ? cellHeight
			: ((row < (int)cellHeights.size()) ?
					cellHeights[row] : (Coordinate)-1.);
}
//-----------------------------------------------------------------------------
template <class LT>
Dimension BasicListUI<LT>::getPreferredSize(AComponentPtr c) {
	maybeUpdateLayoutState();
	ListTypePtr list = getListPtr();
	int lastRow = list->ListType::ListModel::getSize() - 1;
	if (lastRow < 0) {
		return Dimension(0, 0);
	}

	Insets insets = list->getInsets();
	Coordinate width = cellWidth * columnCount + insets.left() + insets.right();
	Coordinate height;

	if (layoutOrientation != ListConstants::VERTICAL) {
		height = preferredHeight;
	} else {
		Rectangle bounds = getCellBounds(*list, lastRow);

		if (bounds != NULL_RECTANGLE) {
			height = bounds.x0().y() + bounds.height() + insets.bottom();
		} else {
			height = 0.;
		}
	}
	return Dimension(width, height);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::updateHorizontalLayoutState(
		const Coordinate &fixedCellWidth,
		const Coordinate &fixedCellHeight)
{
	ListTypePtr list = getListPtr();
	int visRows = list->getVisibleRowCount();
	int dataModelSize = (int)list->ListType::ListModel::getSize();
	Insets insets = list->getInsets();

	listHeight = list->getHeight();
	listWidth = list->getWidth();

	if (dataModelSize == 0) {
		rowsPerColumn = columnCount = 0;
		preferredHeight = insets.top() + insets.bottom();
		return;
	}

	Coordinate height;

	if (fixedCellHeight != -1) {
		height = fixedCellHeight;
	} else {
		// Determine the max of the renderer heights.
		Coordinate maxHeight = 0;
		if (cellHeights.size() > 0) {
			maxHeight = cellHeights[cellHeights.size() - 1];
			for (int counter = cellHeights.size() - 2; counter >= 0; counter--)
			{
				maxHeight = std::max(maxHeight, cellHeights[counter]);
			}
		}
		height = cellHeight = maxHeight;
		cellHeights.clear();
	}
	// The number of rows is either determined by the visible row
	// count, or by the height of the list.
	rowsPerColumn = dataModelSize;
	if (visRows > 0) {
		rowsPerColumn = visRows;
		columnCount = std::max(1, dataModelSize / rowsPerColumn);
		if (dataModelSize > 0 && dataModelSize > rowsPerColumn && dataModelSize
				% rowsPerColumn != 0) {
			columnCount++;
		}
		if (layoutOrientation == ListConstants::HORIZONTAL_WRAP) {
			// Because HORIZONTAL_WRAP flows differently, the
			// rowsPerColumn needs to be adjusted.
			rowsPerColumn = (dataModelSize / columnCount);
			if (dataModelSize % columnCount > 0) {
				rowsPerColumn++;
			}
		}
	} else if (layoutOrientation == ListConstants::VERTICAL_WRAP && height != 0)
	{
		rowsPerColumn = std::max(1,
				(int)((listHeight - insets.top() - insets.bottom()) / height));
		columnCount = std::max(1, dataModelSize / rowsPerColumn);
		if (dataModelSize > 0 && dataModelSize > rowsPerColumn && dataModelSize
				% rowsPerColumn != 0) {
			columnCount++;
		}
	} else if (layoutOrientation ==
			ListConstants::HORIZONTAL_WRAP && cellWidth > 0
			&& listWidth > 0)
	{
		columnCount = std::max(1,
				(int)((listWidth - insets.left() - insets.right()) / cellWidth));
		rowsPerColumn = dataModelSize / columnCount;
		if (dataModelSize % columnCount > 0) {
			rowsPerColumn++;
		}
	}
	preferredHeight =
			rowsPerColumn * cellHeight + insets.top() + insets.bottom();
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::convertLocationToRow(const Coordinate &x,
		const Coordinate &y0, bool closest) const
{
	ListTypePtr list = getListPtr();
	int size = (int)list->ListType::ListModel::getSize();

	if (size <= 0) {
		return -1;
	}
	Insets insets = list->getInsets();
	if (cellHeights.empty()) {
		int row = (int)
				((cellHeight == 0.) ? 0. : ((y0 - insets.top()) / cellHeight));
		if (closest) {
			if (row < 0) {
				row = 0;
			} else if (row >= size) {
				row = size - 1;
			}
		}
		return row;
	} else if (size > (int)cellHeights.size()) {
		return -1;
	} else {
		int y = insets.top();
		int row = 0;

		if (closest && y0 < y) {
			return 0;
		}
		int i;
		for (i = 0; i < size; ++i) {
			if ((y0 >= y) && (y0 < y + cellHeights[i])) {
				return row;
			}
			y += cellHeights[i];
			row += 1;
		}
		return i - 1;
	}
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::convertModelToRow(int index) const {
	ListTypePtr list = getListPtr();
	int size = (int)list->ListType::ListModel::getSize();

	if ((index < 0) || (index >= size)) {
		return -1;
	}

	if (layoutOrientation !=
			ListConstants::VERTICAL && columnCount > 1 && rowsPerColumn > 0)
	{
		if (layoutOrientation == ListConstants::VERTICAL_WRAP) {
			return index % rowsPerColumn;
		}
		return index / columnCount;
	}
	return index;
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::convertModelToColumn(int index) const {
	ListTypePtr list = getListPtr();
	int size = (int)list->ListType::ListModel::getSize();

	if ((index < 0) || (index >= size)) {
		return -1;
	}

	if (layoutOrientation != ListConstants::VERTICAL &&
			rowsPerColumn > 0 && columnCount > 1)
	{
		if (layoutOrientation == ListConstants::VERTICAL_WRAP) {
			return index / rowsPerColumn;
		}
		return index % columnCount;
	}
	return 0;
}
//-----------------------------------------------------------------------------
template <class LT>
int BasicListUI<LT>::convertLocationToModel(const Coordinate &x,
		const Coordinate &y) const
{
	int row = convertLocationToRow(x, y, true);
	int column = convertLocationToColumn(x, y);

	if (row >= 0 && column >= 0) {
		return getModelIndex(column, row);
	}
	return -1;
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::onSelectionStateChanged(void *src,
		const events::ListSelectionEvent &ev)
{
	//updateLayoutStateNeeded |= modelChanged;
	//redrawList();
	ListTypePtr list = getListPtr();
	maybeUpdateLayoutState();
	int size = (int)list->ListType::ListModel::getSize();
	int firstIndex = std::min(size - 1, std::max(ev.getFirstIndex(), 0));
	int lastIndex = std::min(size - 1, std::max(ev.getLastIndex(), 0));

	Rectangle bounds = getCellBounds(list, firstIndex, lastIndex);

	if (bounds != NULL_RECTANGLE) {
		list->redraw(bounds);
	}
}
///////////////////////////////////////////////////////////////////////////////
// MouseEvent listeners
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::onMouse(void *src, const events::MouseEvent &ev)
{
	enum { Filter = events::MouseEvent::DISCO_MOUSE_PRESSED };
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::mousePressed(const events::MouseEvent &ev)
{
	adjustSelection(ev);
}
//-----------------------------------------------------------------------------
template <class LT>
void BasicListUI<LT>::adjustSelection(const events::MouseEvent &ev)
{
	Point2D point = ev.getLocation();
	ListTypePtr list = getListPtr();
	int row = list->locationToIndex(point);
	list->setSelectionInterval(row, row);
}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICLISTUI_H */
