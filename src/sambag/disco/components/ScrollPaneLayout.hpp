/*
 * ScrollPaneLayout.hpp
 *
 *  Created on: Fri Jul  6 09:48:21 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SCROLLPANELAYOUT_H
#define SAMBAG_SCROLLPANELAYOUT_H

#include <boost/shared_ptr.hpp>
#include "ALayoutManager.hpp"
#include "Forward.hpp"
#include "ScrollPane.hpp"

namespace sambag {
namespace disco {
namespace components {
//=============================================================================
/** 
 * @class ScrollPaneLayout.
 */
class ScrollPaneLayout: public ALayoutManager {
	//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ScrollPaneLayout> Ptr;
	typedef ScrollPane::VerticalScrollbarPolicy VerticalScrollbarPolicy;
	typedef ScrollPane::HorizontalScrollbarPolicy HorizontalScrollbarPolicy;
	typedef ScrollPane::AScrollbarPtr AScrollbarPtr;
protected:
	//-------------------------------------------------------------------------
	ScrollPaneLayout() :
		vsbPolicy(ScrollPane::VERTICAL_SCROLLBAR_AS_NEEDED),
		hsbPolicy(ScrollPane::HORIZONTAL_SCROLLBAR_AS_NEEDED)
	{
	}
	//-------------------------------------------------------------------------
	/**
	 * Removes an existing AComponentPtr.
	 */
	AComponentPtr addSingletonComponent(AComponentPtr oldC,
			AComponentPtr newC);
	//-------------------------------------------------------------------------
	/**
	 * The scrollpane's viewport child.
	 * Default is an empty <code>ViewportPtr</code>.
	 * @see ScrollPane#setViewport
	 */
	ViewportPtr viewport;
	//-------------------------------------------------------------------------
	/**
	 * The scrollpane's vertical scrollbar child.
	 * Default is a <code>ScrollbarPtr</code>.
	 * @see ScrollPane#setVerticalScrollBar
	 */
	AScrollbarPtr vsb;
	//-------------------------------------------------------------------------
	/**
	 * The scrollpane's horizontal scrollbar child.
	 * Default is a <code>ScrollbarPtr</code>.
	 * @see ScrollPane#setHorizontalScrollBar
	 */
	AScrollbarPtr hsb;
	//-------------------------------------------------------------------------
	/**
	 * The row header child.  Default is <code>null</code>.
	 * @see ScrollPane#setRowHeader
	 */
	ViewportPtr rowHead;
	//-------------------------------------------------------------------------
	/**
	 * The column header child.  Default is <code>null</code>.
	 * @see ScrollPane#setColumnHeader
	 */
	ViewportPtr colHead;
	//-------------------------------------------------------------------------
	/**
	 * The AComponentPtr to display in the lower left corner.
	 * Default is <code>null</code>.
	 * @see ScrollPane#setCorner
	 */
	AComponentPtr lowerLeft;
	//-------------------------------------------------------------------------
	/**
	 * The AComponentPtr to display in the lower right corner.
	 * Default is <code>null</code>.
	 * @see ScrollPane#setCorner
	 */
	AComponentPtr lowerRight;
	//-------------------------------------------------------------------------
	/**
	 * The AComponentPtr to display in the upper left corner.
	 * Default is <code>null</code>.
	 * @see ScrollPane#setCorner
	 */
	AComponentPtr upperLeft;
	//-------------------------------------------------------------------------
	/**
	 * The AComponentPtr to display in the upper right corner.
	 * Default is <code>null</code>.
	 * @see ScrollPane#setCorner
	 */
	AComponentPtr upperRight;
	//-------------------------------------------------------------------------
	/**
	 * The display policy for the vertical scrollbar.
	 * The default is <code>ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED</code>.
	 * <p>
	 * This field is obsolete, please use the <code>ScrollPane</code> field instead.
	 *
	 * @see ScrollPane#setVerticalScrollBarPolicy
	 */
	VerticalScrollbarPolicy vsbPolicy;
	//-------------------------------------------------------------------------
	/**
	 * The display policy for the horizontal scrollbar.
	 * The default is <code>ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED</code>.
	 * <p>
	 * This field is obsolete, please use the <code>ScrollPane</code> field instead.
	 *
	 * @see ScrollPane#setHorizontalScrollBarPolicy
	 */
	HorizontalScrollbarPolicy hsbPolicy;
private:
	//-------------------------------------------------------------------------
	void adjustForVSB(bool wantsVSB, Rectangle &available,
			Rectangle &vsbR, const Insets &vpbInsets);
	//-------------------------------------------------------------------------
	/**
	 * Adjusts the <code>Rectangle</code> <code>available</code> based on if
	 * the horizontal scrollbar is needed (<code>wantsHSB</code>).
	 * The location of the hsb is updated in <code>hsbR</code>, and
	 * the viewport border insets (<code>vpbInsets</code>) are used to offset
	 * the hsb.  This is only called when <code>wantsHSB</code> has
	 * changed, eg you shouldn't invoked adjustForHSB(true) twice.
	 */
	void adjustForHSB(bool wantsHSB, Rectangle &available,
			Rectangle &hsbR, const Insets &vpbInsets);
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new ScrollPaneLayout());
	}
	//-------------------------------------------------------------------------
	/**
	 * Adds the specified AComponentPtr to the layout.
	 * @param s
	 * @param c
	 */
	virtual void addLayoutComponent(AComponentPtr comp,
			 ArbitraryType::Ptr constraint = ArbitraryType::Ptr());
	//-------------------------------------------------------------------------
	/**
	 * Returns the ViewportPtr object that is the column header.
	 * @return
	 */
	ViewportPtr getColumnHeader() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the AComponentPtr at the specified corner.
	 * @param key
	 * @return
	 */
	AComponentPtr getCorner(ScrollPane::Area loc) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the ScrollbarPtr object that handles horizontal scrolling.
	 * @return
	 */
	AScrollbarPtr getHorizontalScrollBar() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the horizontal scrollbar-display policy.
	 * @return
	 */
	HorizontalScrollbarPolicy getHorizontalScrollBarPolicy() const;
	//-------------------------------------------------------------------------
	/**
	 *
	 * @return
	 */
	ViewportPtr getRowHeader() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the ScrollbarPtr object that handles vertical scrolling.
	 * @return
	 */
	AScrollbarPtr getVerticalScrollBar() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the vertical scrollbar-display policy.
	 * @return
	 */
	VerticalScrollbarPolicy getVerticalScrollBarPolicy() const;
	//-------------------------------------------------------------------------
	/**
	 * 	Returns the ViewportPtr object that displays the scrollable contents.
	 Rectangle	getViewportBorderBounds(ScrollPane scrollpane.)
	 * @return
	 */
	ViewportPtr getViewport() const;
	//-------------------------------------------------------------------------
	/**
	 * Lays out the scrollpane.
	 * Dimension	minimumLayoutSize(AContainerPtr parent)
	 * The minimum size of a ScrollPane is the size of the insets plus minimum
	 * size of the viewport, plus the scrollpane's viewportBorder insets, plus
	 * the minimum size of the visible headers, plus the minimum size of the
	 * scrollbars whose displayPolicy isn't NEVER.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * The preferred size of a ScrollPane is the size of the insets, plus
	 * the preferred size of the viewport, plus the preferred size of the
	 * visible headers, plus the preferred size of the scrollbars that will
	 * appear given the current view and the current scrollbar displayPolicies.
	 * @param parent
	 * @return
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	virtual Dimension minimumLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified AComponentPtr from the layout.
	 * @param c
	 */
	virtual void removeLayoutComponent(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Sets the horizontal scrollbar-display policy.
	 * @param x
	 */
	void setHorizontalScrollBarPolicy(HorizontalScrollbarPolicy x);
	//-------------------------------------------------------------------------
	/**
	 * Sets the vertical scrollbar-display policy.
	 * @param x
	 */
	void setVerticalScrollBarPolicy(VerticalScrollbarPolicy x);
	//-------------------------------------------------------------------------
	/**
	 * This method is invoked after the ScrollPaneLayout is
	 * set as the LayoutManager of a ScrollPane.
	 * @param sp
	 */
	void syncWithScrollPane(ScrollPanePtr sp);
}; // ScrollPaneLayout
}
}
} // namespace(s)

#endif /* SAMBAG_SCROLLPANELAYOUT_H */
