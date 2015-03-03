/*
 * GridLayout.hpp
 *
 *  Created on: Tue Sep 18 13:18:55 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GRIDLAYOUT_H
#define SAMBAG_GRIDLAYOUT_H

#include <boost/shared_ptr.hpp>
#include "ALayoutManager.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
 * @class GridLayout.
 *  The <code>GridLayout</code> class is a layout manager that
 * lays out a container's components in a rectangular grid.
 * The container is divided into equal-sized rectangles,
 * and one component is placed in each rectangle.
 */
class GridLayout : public ALayoutManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<GridLayout> Ptr;
protected:
private:
	//-------------------------------------------------------------------------
	/**
	 * This is the horizontal gap (in pixels) which specifies the space
	 * between columns.  They can be changed at any time.
	 * This should be a non-negative integer.
	 * @see #getHgap()
	 * @see #setHgap(int)
	 */
	Coordinate hgap;
	//-------------------------------------------------------------------------
	/**
	 * This is the vertical gap (in pixels) which specifies the space
	 * between rows.  They can be changed at any time.
	 * This should be a non negative integer.
	 * @see #getVgap()
	 * @see #setVgap(int)
	 */
	Coordinate vgap;
	//-------------------------------------------------------------------------
	/**
	 * This is the number of rows specified for the grid.  The number
	 * of rows can be changed at any time.
	 * This should be a non negative integer, where '0' means
	 * 'any number' meaning that the number of Rows in that
	 * dimension depends on the other dimension.
	 * @see #getRows()
	 * @see #setRows(int)
	 */
	sambag::com::ArithmeticWrapper<int> rows;
	//-------------------------------------------------------------------------
	/**
	 * This is the number of columns specified for the grid.  The number
	 * of columns can be changed at any time.
	 * This should be a non negative integer, where '0' means
	 * 'any number' meaning that the number of Columns in that
	 * dimension depends on the other dimension.
	 * @see #getColumns()
	 * @see #setColumns(int)
	 */
	sambag::com::ArithmeticWrapper<int> cols;
	//-------------------------------------------------------------------------
	/**
	 * Creates a grid layout with a default of one column per component,
	 * in a single row.
	 */
protected:
	//-------------------------------------------------------------------------
	GridLayout(){}
	//-------------------------------------------------------------------------
	GridLayout(int rows, int cols,
		const Coordinate & hgap, const Coordinate & vgap);
public:
	//-------------------------------------------------------------------------
	static Ptr create(int rows=0, int cols=1,
			const Coordinate & hgap=0., const Coordinate & vgap=0.);
	//-------------------------------------------------------------------------
	/**
	 * Gets the number of rows in this layout.
	 * @return    the number of rows in this layout
	 */
	int getRows() {
		return rows;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the number of rows in this layout to the specified value.
	 * @param        rows   the number of rows in this layout
	 * @exception    IllegalArgumentException  if the value of both
	 *               <code>rows</code> and <code>cols</code> is set to zero
	 */
	void setRows(int rows);
	//-------------------------------------------------------------------------
	/**
	 * Gets the number of columns in this layout.
	 * @return     the number of columns in this layout
	 */
	int getColumns() {
		return cols;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the number of columns in this layout to the specified value.
	 * Setting the number of columns has no affect on the layout
	 * if the number of rows specified by a constructor or by
	 * the <tt>setRows</tt> method is non-zero. In that case, the number
	 * of columns displayed in the layout is determined by the total
	 * number of components and the number of rows specified.
	 * @param        cols   the number of columns in this layout
	 * @exception    IllegalArgumentException  if the value of both
	 *               <code>rows</code> and <code>cols</code> is set to zero
	 */
	void setColumns(int cols);
	//-------------------------------------------------------------------------
	/**
	 * Gets the horizontal gap between components.
	 * @return       the horizontal gap between components
	 */
	Coordinate getHgap() {
		return hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the horizontal gap between components to the specified value.
	 * @param        hgap   the horizontal gap between components
	 */
	void setHgap(const Coordinate &hgap);
	//-------------------------------------------------------------------------
	/**
	 * Gets the vertical gap between components.
	 * @return       the vertical gap between components
	 */
	Coordinate getVgap() {
		return vgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the vertical gap between components to the specified value.
	 * @param         vgap  the vertical gap between components
	 */
	void setVgap(const Coordinate &vgap);
	//-------------------------------------------------------------------------
	/**
	 * Determines the preferred size of the container argument using
	 * this grid layout.
	 * <p>
	 * The preferred width of a grid layout is the largest preferred
	 * width of all of the components in the container times the number of
	 * columns, plus the horizontal padding times the number of columns
	 * minus one, plus the left and right insets of the target container.
	 * <p>
	 * The preferred height of a grid layout is the largest preferred
	 * height of all of the components in the container times the number of
	 * rows, plus the vertical padding times the number of rows minus one,
	 * plus the top and bottom insets of the target container.
	 *
	 * @param     parent   the container in which to do the layout
	 * @return    the preferred dimensions to lay out the
	 *                      subcomponents of the specified container
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Determines the minimum size of the container argument using this
	 * grid layout.
	 * <p>
	 * The minimum width of a grid layout is the largest minimum width
	 * of all of the components in the container times the number of columns,
	 * plus the horizontal padding times the number of columns minus one,
	 * plus the left and right insets of the target container.
	 * <p>
	 * The minimum height of a grid layout is the largest minimum height
	 * of all of the components in the container times the number of rows,
	 * plus the vertical padding times the number of rows minus one, plus
	 * the top and bottom insets of the target container.
	 *
	 * @param       parent   the container in which to do the layout
	 * @return      the minimum dimensions needed to lay out the
	 *                      subcomponents of the specified container
	 * @see         java.awt.GridLayout#preferredLayoutSize
	 * @see         java.awt.Container#doLayout
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Lays out the specified container using this layout.
	 * <p>
	 * This method reshapes the components in the specified target
	 * container in order to satisfy the constraints of the
	 * <code>GridLayout</code> object.
	 * <p>
	 * The grid layout manager determines the size of individual
	 * components by dividing the free space in the container into
	 * equal-sized portions according to the number of rows and columns
	 * in the layout. The container's free space equals the container's
	 * size minus any insets and any specified horizontal or vertical
	 * gap. All components in a grid layout are given the same size.
	 *
	 * @param      parent   the container in which to do the layout
	 * @see        java.awt.Container
	 * @see        java.awt.Container#doLayout
	 */
	virtual void layoutContainer(AContainerPtr parent);
}; // GridLayout
}}} // namespace(s)

#endif /* SAMBAG_GRIDLAYOUT_H */
