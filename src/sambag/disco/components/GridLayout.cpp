/*
 * GridLayout.cpp
 *
 *  Created on: Tue Sep 18 13:18:55 2012
 *      Author: Johannes Unger
 */

#include "GridLayout.hpp"
#include <sambag/com/Exception.hpp>
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class GridLayout
//=============================================================================
//-----------------------------------------------------------------------------
GridLayout::GridLayout(int rows, int cols,
		const Coordinate & hgap, const Coordinate & vgap) :
hgap(hgap),
vgap(vgap),
rows(rows),
cols(cols)
{
	if ((rows == 0) && (cols == 0)) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalArgumentException,
			"rows and cols cannot both be zero"
		);
	}
}
//-----------------------------------------------------------------------------
GridLayout::Ptr GridLayout::create(int rows, int cols,
		const Coordinate & hgap, const Coordinate & vgap)
{

	Ptr res(new GridLayout(rows, cols, hgap, vgap));
	return res;
}
//-----------------------------------------------------------------------------
void GridLayout::setRows(int rows) {
	if ((rows == 0) && (cols == 0)) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalArgumentException,
			"rows and cols cannot both be zero"
		);
	}
	this->rows = rows;
}
//-----------------------------------------------------------------------------
void GridLayout::setColumns(int cols) {
	if ((rows == 0) && (cols == 0)) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalArgumentException,
			"rows and cols cannot both be zero"
		);
	}
	this->cols = cols;
}
//-----------------------------------------------------------------------------
void GridLayout::setHgap(const Coordinate &hgap) {
	this->hgap = hgap;
}
//------------------------------------------------------------------------------
void GridLayout::setVgap(const Coordinate &vgap) {
	this->vgap = vgap;
}
//-----------------------------------------------------------------------------
Dimension GridLayout::preferredLayoutSize(AContainerPtr parent) {
	Insets insets = parent->getInsets();
	int ncomponents = (int)parent->getComponentCount();
	int nrows = rows;
	int ncols = cols;

	if (nrows > 0) {
		ncols = (ncomponents + nrows - 1) / nrows;
	} else {
		nrows = (ncomponents + ncols - 1) / ncols;
	}
	Number w = 0;
	Number h = 0;
	for (int i = 0; i < ncomponents; ++i) {
		AComponentPtr comp = parent->getComponent(i);
		Dimension d = comp->getPreferredSize();
		if (w < d.width()) {
			w = d.width();
		}
		if (h < d.height()) {
			h = d.height();
		}
	}
	Dimension res(
			insets.left() + insets.right() + ncols * w + (ncols - 1) * hgap,
			insets.top() + insets.bottom() + nrows * h + (nrows - 1) * vgap);
	return res;
}
//-----------------------------------------------------------------------------
Dimension GridLayout::minimumLayoutSize(AContainerPtr parent) {
	Insets insets = parent->getInsets();
	int ncomponents = (int)parent->getComponentCount();
	int nrows = rows;
	int ncols = cols;

	if (nrows > 0) {
		ncols = (ncomponents + nrows - 1) / nrows;
	} else {
		nrows = (ncomponents + ncols - 1) / ncols;
	}
	Number w = 0;
	Number h = 0;
	for (int i = 0; i < ncomponents; ++i) {
		AComponentPtr comp = parent->getComponent(i);
		Dimension d = comp->getMinimumSize();
		if (w < d.width()) {
			w = d.width();
		}
		if (h < d.height()) {
			h = d.height();
		}
	}
	return Dimension(
			insets.left() + insets.right() + ncols * w + (ncols - 1) * hgap,
			insets.top() + insets.bottom() + nrows * h + (nrows - 1) * vgap);
}
//-----------------------------------------------------------------------------
void GridLayout::layoutContainer(AContainerPtr parent) {
	Insets insets = parent->getInsets();
	int ncomponents = parent->getComponentCount();
	int nrows = rows;
	int ncols = cols;

	if (ncomponents == 0) {
		return;
	}
	if (nrows > 0) {
		ncols = (ncomponents + nrows - 1.) / nrows;
	} else {
		nrows = (ncomponents + ncols - 1.) / ncols;
	}
	// To position components in the center we should:
	// 1. get an amount of extra space within Container
	// 2. incorporate half of that value to the left/top position
	// Note that we use trancating division for widthOnComponent
	// The reminder goes to extraWidthAvailable
	Number totalGapsWidth = (ncols - 1.) * hgap;
	Number widthWOInsets = parent->getWidth() - (insets.left() + insets.right());
	Number widthOnComponent = (widthWOInsets - totalGapsWidth) / ncols;
	Number extraWidthAvailable = (widthWOInsets - (widthOnComponent * ncols
			+ totalGapsWidth)) / 2.;

	Number totalGapsHeight = (nrows - 1) * vgap;
	Number heightWOInsets = parent->getHeight() - (insets.top()+ insets.bottom());
	Number heightOnComponent = (heightWOInsets - totalGapsHeight) / nrows;
	Number extraHeightAvailable = (heightWOInsets - (heightOnComponent * nrows
			+ totalGapsHeight)) / 2.;

	for (
			int c = 0, x = (int)(insets.left() + extraWidthAvailable);
			c < (int)ncols;
			c++, x += (int)(widthOnComponent + hgap)
		)
	{
		for (
			int r = 0, y = (int)(insets.top() + extraHeightAvailable);
			r < (int)nrows;
			r++, y += (int)(heightOnComponent + vgap)
		)
		{
			int i = r * (int)(ncols) + c;
			if (i < ncomponents) {
				parent->getComponent(i)->setBounds(
					Rectangle(x, y, widthOnComponent,
						heightOnComponent
				));
			}
		}
	}

}
}}} // namespace(s)
