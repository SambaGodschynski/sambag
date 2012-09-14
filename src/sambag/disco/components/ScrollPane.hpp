/*
 * ScrollPane.hpp
 *
 *  Created on: Thu Jul  5 16:50:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SCROLLPANE_H
#define SAMBAG_SCROLLPANE_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include "Forward.hpp"
#include "Scrollbar.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ScrollPane.
  */
class ScrollPane : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	/**
	 * TODO: it would be more consequence to impl. the ScrollPane class using
	 * ScrollBarModel template param.
	 */
	typedef DefaultBoundedRangeModel ScrollbarModel;
	//-------------------------------------------------------------------------
	enum Area {
		/**
		 * Identifies a "viewport" or display area, within which
		 * scrolled contents are visible.
		 */
		VIEWPORT,
		/** Identifies a vertical scrollbar. */
		VERTICAL_SCROLLBAR,
		/** Identifies a horizonal scrollbar. */
		HORIZONTAL_SCROLLBAR,
		/**
		 * Identifies the area along the left side of the viewport between the
		 * upper left corner and the lower left corner.
		 */
		ROW_HEADER,
		/**
		 * Identifies the area at the top the viewport between the
		 * upper left corner and the upper right corner.
		 */
		COLUMN_HEADER,
		/** Identifies the lower left corner of the viewport. */
		LOWER_LEFT_CORNER,
		/** Identifies the lower right corner of the viewport. */
		LOWER_RIGHT_CORNER,
		/** Identifies the upper left corner of the viewport. */
		UPPER_LEFT_CORNER,
		/** Identifies the upper right corner of the viewport. */
		UPPER_RIGHT_CORNER,
		/** Identifies the lower leading edge corner of the viewport.
		 * The leading edge is determined relative to the Scroll Pane's
		 * ComponentOrientation property.
		 */
		LOWER_LEADING_CORNER,
		/** Identifies the lower trailing edge corner of the viewport.
		 * The trailing edgeis determined relative to the
		 * Scroll Pane's ComponentOrientation property.
		 */
		LOWER_TRAILING_CORNER,
		/** Identifies the upper leading edge corner of the viewport.
		 * The leading edge is determined relative to the Scroll Pane's
		 * ComponentOrientation property.
		 */
		UPPER_LEADING_CORNER,
		/** Identifies the upper trailing edge corner of the viewport.
		 * The trailing edge is determined relative to the Scroll Pane's
		 * ComponentOrientation property.
		 */
		UPPER_TRAILING_CORNER
	};

	enum VerticalScrollbarPolicy {
		/**
		 * Used to set the vertical scroll bar policy so that
		 * vertical scrollbars are displayed only when needed.
		 */
		VERTICAL_SCROLLBAR_AS_NEEDED,
		/**
		 * Used to set the vertical scroll bar policy so that
		 * vertical scrollbars are never displayed.
		 */
		VERTICAL_SCROLLBAR_NEVER,
		/**
		 * Used to set the vertical scroll bar policy so that
		 * vertical scrollbars are always displayed.
		 */
		VERTICAL_SCROLLBAR_ALWAYS
	};

	enum HorizontalScrollbarPolicy {
		/**
		 * Used to set the horizontal scroll bar policy so that
		 * horizontal scrollbars are displayed only when needed.
		 */
		HORIZONTAL_SCROLLBAR_AS_NEEDED,
		/**
		 * Used to set the horizontal scroll bar policy so that
		 * horizontal scrollbars are never displayed.
		 */
		HORIZONTAL_SCROLLBAR_NEVER,
		/**
		 * Used to set the horizontal scroll bar policy so that
		 * horizontal scrollbars are always displayed.
		 */
		HORIZONTAL_SCROLLBAR_ALWAYS
	};
	//-------------------------------------------------------------------------
	/** Identifies the vertical scroll bar policy property. */
	static const std::string PROPERTY_VERTICAL_SCROLLBAR_POLICY;
	//-------------------------------------------------------------------------
	/** Identifies the horizontal scroll bar policy property. */
	static const std::string PROPERTY_HORIZONTAL_SCROLLBAR_POLICY;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_VERTICAL_SCROLLBAR;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_HORIZONTAL_SCROLLBAR;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_VIEWPORT;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_VIEWPORT_BORDER;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_ROWHEADER;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_COLUMNHEADER;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_CORNER;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_WHEELSCROLLING_ENABLED;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ScrollPane> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	ScrollPane();
private:
	//-------------------------------------------------------------------------
	IBorder::Ptr viewportBorder;
	//-------------------------------------------------------------------------
	/*
	 * State flag for mouse wheel scrolling
	 */
	sambag::com::ArithmeticWrapper<bool, true> wheelScrollState;
protected:
	//-------------------------------------------------------------------------
	/**
	 *
	 * By default <code>ScrollPane</code> creates scrollbars
	 * that are instances
	 * of this class.  <code>Scrollbar</code> overrides the
	 * <code>getUnitIncrement</code> and <code>getBlockIncrement</code>
	 * methods so that, if the viewport's view is a <code>Scrollable</code>,
	 * the view is asked to compute these values. Unless
	 * the unit/block increment have been explicitly set.
	 * <p>
	 */
	class MyScrollbar;
	//-------------------------------------------------------------------------
	friend class MyScrollbar;
	//-------------------------------------------------------------------------
	/**
	 * Returns a new <code>Viewport</code> by default.
	 * Used to create the
	 * viewport (as needed) in <code>setViewportView</code>,
	 * <code>setRowHeaderView</code>, and <code>setColumnHeaderView</code>.
	 * Subclasses may override this method to return a subclass of
	 * <code>Viewport</code>.
	 *
	 * @return a new <code>Viewport</code>
	 */
	virtual ViewportPtr createViewport();
	//-------------------------------------------------------------------------
	/**
	 * The display policy for the vertical scrollbar.
	 * The default is
	 * <code>ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED</code>.
	 * @see #setVerticalScrollBarPolicy
	 */
	VerticalScrollbarPolicy verticalScrollBarPolicy;
	//-------------------------------------------------------------------------
	/**
	 * The display policy for the horizontal scrollbar.
	 * The default is
	 * <code>ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED</code>.
	 * @see #setHorizontalScrollBarPolicy
	 */
	HorizontalScrollbarPolicy horizontalScrollBarPolicy;
	//-------------------------------------------------------------------------
	/**
	 * The scrollpane's viewport child.  Default is an empty
	 * <code>JViewport</code>.
	 * @see #setViewport
	 */
	ViewportPtr viewport;
	//-------------------------------------------------------------------------
	/**
	 * The scrollpane's vertical scrollbar child.
	 * Default is a <code>JScrollBar</code>.
	 * @see #setVerticalScrollBar
	 */
	ScrollbarPtr verticalScrollBar;
	//-------------------------------------------------------------------------
	/**
	 * The scrollpane's horizontal scrollbar child.
	 * Default is a <code>JScrollBar</code>.
	 * @see #setHorizontalScrollBar
	 */
	ScrollbarPtr horizontalScrollBar;
	//-------------------------------------------------------------------------
	/**
	 * The row header child.  Default is <code>null</code>.
	 * @see #setRowHeader
	 */
	ViewportPtr rowHeader;
	//-------------------------------------------------------------------------
	/**
	 * The column header child.  Default is <code>null</code>.
	 * @see #setColumnHeader
	 */
	ViewportPtr columnHeader;
	//-------------------------------------------------------------------------
	/**
	 * The component to display in the lower left corner.
	 * Default is <code>null</code>.
	 * @see #setCorner
	 */
	AComponentPtr lowerLeft;
	//-------------------------------------------------------------------------
	/**
	 * The component to display in the lower right corner.
	 * Default is <code>null</code>.
	 * @see #setCorner
	 */
	AComponentPtr lowerRight;
	//-------------------------------------------------------------------------
	/**
	 * The component to display in the upper left corner.
	 * Default is <code>null</code>.
	 * @see #setCorner
	 */
	AComponentPtr upperLeft;
	//-------------------------------------------------------------------------
	/**
	 * The component to display in the upper right corner.
	 * Default is <code>null</code>.
	 * @see #setCorner
	 */
	AComponentPtr upperRight;
public:
	//-------------------------------------------------------------------------
	void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	virtual ScrollbarPtr createHorizontalScrollBar();
	//-------------------------------------------------------------------------
	virtual ScrollbarPtr createVerticalScrollBar();
	//-------------------------------------------------------------------------
	/**
	 * Adds the scrollbar that controls the viewport's horizontal view
	 * position to the scrollpane.
	 * This is usually unnecessary, as <code>JScrollPane</code> creates
	 * horizontal and vertical scrollbars by default.
	 * @param horizontalScrollBar the horizontal scrollbar to be added
	 * @see #createHorizontalScrollBar
	 * @see #getHorizontalScrollBar
	 */
	virtual void setHorizontalScrollBar(ScrollbarPtr scrollbar);
	//-------------------------------------------------------------------------
	/**
	 * Adds the scrollbar that controls the viewport's horizontal view
	 * position to the scrollpane.
	 * This is usually unnecessary, as <code>JScrollPane</code> creates
	 * horizontal and vertical scrollbars by default.
	 * @param horizontalScrollBar the horizontal scrollbar to be added
	 * @see #createHorizontalScrollBar
	 * @see #getHorizontalScrollBar
	 */
	virtual void setVerticalScrollBar(ScrollbarPtr scrollbar);
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr< AScrollbar<ScrollbarModel> > AScrollbarPtr;
	//-------------------------------------------------------------------------
	/**
	 * Returns the horizontal scroll bar that controls the viewport's
	 * horizontal view position.
	 *
	 * @return the <code>horizontalScrollBar</code> property
	 * @see #setHorizontalScrollBar
	 */
	AScrollbarPtr getHorizontalScrollBar() const {
		return horizontalScrollBar;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the vertical scroll bar that controls the viewport's
	 * vertical view position.
	 *
	 * @return the <code>verticalScrollBar</code> property
	 * @see #setHorizontalScrollBar
	 */
	AScrollbarPtr getVerticalScrollBar() const {
		return verticalScrollBar;
	}
	//-------------------------------------------------------------------------
	virtual void setLayout(ALayoutManagerPtr layout);
	//-------------------------------------------------------------------------
	/**
	* Returns the current <code>Viewport</code>.
	*
	* @see #setViewport
	* @return the <code>viewport</code> property
	*/
	virtual ViewportPtr getViewport() const;
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<ScrollPane>(Super::getPtr());
	}
	//-------------------------------------------------------------------------
	static Ptr create(AComponentPtr view, VerticalScrollbarPolicy vsbPolicy,
			HorizontalScrollbarPolicy hsbPolicy);
	//-------------------------------------------------------------------------
	static Ptr create(VerticalScrollbarPolicy vsbPolicy,
			HorizontalScrollbarPolicy hsbPolicy)
	{
		return create(AComponentPtr(), vsbPolicy, hsbPolicy);
	}
	//-------------------------------------------------------------------------
	static Ptr create(AComponentPtr view) {
		return create(view, VERTICAL_SCROLLBAR_AS_NEEDED,
				HORIZONTAL_SCROLLBAR_AS_NEEDED);
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return create(AComponentPtr(), VERTICAL_SCROLLBAR_AS_NEEDED,
				HORIZONTAL_SCROLLBAR_AS_NEEDED);
	}
	//-------------------------------------------------------------------------
	/**
	 * Overridden to return true so that any calls to <code>revalidate</code>
	 * on any descendants of this <code>ScrollPane</code> will cause the
	 * entire tree beginning with this <code>ScrollPane</code> to be
	 * validated.
	 *
	 * @return true
	 */
	virtual bool isValidateRoot() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the vertical scroll bar policy value.
	 * @return the <code>verticalScrollBarPolicy</code> property
	 * @see #setVerticalScrollBarPolicy
	 */
	virtual VerticalScrollbarPolicy getVerticalScrollBarPolicy() const;
	//-------------------------------------------------------------------------
	/**
	 * Determines when the vertical scrollbar appears in the scrollpane.
	 * Legal values are:
	 * <ul>
	 * <li><code>ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED</code>
	 * <li><code>ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER</code>
	 * <li><code>ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS</code>
	 * </ul>
	 *
	 * @param policy one of the three values listed above
	 * @exception IllegalArgumentException if <code>policy</code>
	 *                          is not one of the legal values shown above
	 */
	virtual void
	setVerticalScrollBarPolicy(VerticalScrollbarPolicy policy);
	//-------------------------------------------------------------------------
	/**
	 * Returns the horizontal scroll bar policy value.
	 * @return the <code>horizontalScrollBarPolicy</code> property
	 * @see #setHorizontalScrollBarPolicy
	 */
	virtual HorizontalScrollbarPolicy getHorizontalScrollBarPolicy() const;
	//-------------------------------------------------------------------------
	/**
	 * Determines when the horizontal scrollbar appears in the scrollpane.
	 * The options are:<ul>
	 * <li><code>ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED</code>
	 * <li><code>ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER</code>
	 * <li><code>ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS</code>
	 * </ul>
	 *
	 * @param policy one of the three values listed above
	 * @exception IllegalArgumentException if <code>policy</code>
	 *                          is not one of the legal values shown above
	 * @see #getHorizontalScrollBarPolicy
	 */
	virtual void setHorizontalScrollBarPolicy(HorizontalScrollbarPolicy p);
	//-------------------------------------------------------------------------
	/**
	 * Returns the <code>Border</code> object that surrounds the viewport.
	 *
	 * @return the <code>viewportBorder</code> property
	 * @see #setViewportBorder
	 */
	virtual IBorder::Ptr getViewportBorder() const;
	//-------------------------------------------------------------------------
	/**
	 * Adds a border around the viewport.  Note that the border isn't
	 * set on the viewport directly, <code>JViewport</code> doesn't support
	 * the <code>JComponent</code> border property.
	 * Similarly setting the <code>JScrollPane</code>s
	 * viewport doesn't affect the <code>viewportBorder</code> property.
	 * <p>
	 * The default value of this property is computed by the look
	 * and feel implementation.
	 *
	 * @param viewportBorder the border to be added
	 * @see #getViewportBorder
	 * @see #setViewport
	 *
	 * @beaninfo
	 *   preferred: true
	 *       bound: true
	 * description: The border around the viewport.
	 */
	virtual void setViewportBorder(IBorder::Ptr viewportBorder);
	//-------------------------------------------------------------------------
	/**
	 * Returns the bounds of the viewport's border.
	 *
	 * @return a <code>Rectangle</code> object specifying the viewport border
	 */
	virtual Rectangle getViewportBorderBounds() const;
	//-------------------------------------------------------------------------
	/**
	 * Creates a viewport if necessary and then sets its view.  Applications
	 * that don't provide the view directly to the <code>JScrollPane</code>
	 * constructor
	 * should use this method to specify the scrollable child that's going
	 * to be displayed in the scrollpane. For example:
	 * <pre>
	 * JScrollPane scrollpane = new JScrollPane();
	 * scrollpane.setViewportView(myBigComponentToScroll);
	 * </pre>
	 * Applications should not add children directly to the scrollpane.
	 *
	 * @param view the component to add to the viewport
	 * @see #setViewport
	 * @see JViewport#setView
	 */
	virtual void setViewportView(AComponentPtr view);
	//-------------------------------------------------------------------------
	/**
	 * Returns the row header.
	 * @return the <code>rowHeader</code> property
	 * @see #setRowHeader
	 */
	virtual ViewportPtr getRowHeader() const;
	//-------------------------------------------------------------------------
	/**
	 * Removes the old rowHeader, if it exists; if the new rowHeader
	 * isn't <code>null</code>, syncs the y coordinate of its
	 * viewPosition with
	 * the viewport (if there is one) and then adds it to the scroll pane.
	 * <p>
	 * Most applications will find it more convenient to use
	 * <code>setRowHeaderView</code>
	 * to add a row header component and its viewport to the scroll pane.
	 *
	 * @param rowHeader the new row header to be used; if <code>null</code>
	 *          the old row header is still removed and the new rowHeader
	 *          is set to <code>null</code>
	 * @see #getRowHeader
	 * @see #setRowHeaderView
	 *
	 * @beaninfo
	 *        bound: true
	 *       expert: true
	 *  description: The row header child for this scrollpane
	 */
	virtual void setRowHeader(ViewportPtr rowHeader);
	//-------------------------------------------------------------------------
	/**
	 * Creates a row-header viewport if necessary, sets
	 * its view and then adds the row-header viewport
	 * to the scrollpane.  For example:
	 * <pre>
	 * JScrollPane scrollpane = new JScrollPane();
	 * scrollpane.setViewportView(myBigComponentToScroll);
	 * scrollpane.setRowHeaderView(myBigComponentsRowHeader);
	 * </pre>
	 *
	 * @see #setRowHeader
	 * @see JViewport#setView
	 * @param view the component to display as the row header
	 */
	virtual void setRowHeaderView(AComponentPtr view);
	//-------------------------------------------------------------------------
	/**
	 * Returns the column header.
	 * @return the <code>columnHeader</code> property
	 * @see #setColumnHeader
	 */
	virtual ViewportPtr getColumnHeader() const;
	//-------------------------------------------------------------------------
	/**
	 * Removes the old columnHeader, if it exists; if the new columnHeader
	 * isn't <code>null</code>, syncs the x coordinate of its viewPosition
	 * with the viewport (if there is one) and then adds it to the scroll pane.
	 * <p>
	 * Most applications will find it more convenient to use
	 * <code>setColumnHeaderView</code>
	 * to add a column header component and its viewport to the scroll pane.
	 *
	 * @see #getColumnHeader
	 * @see #setColumnHeaderView
	 *
	 * @beaninfo
	 *        bound: true
	 *  description: The column header child for this scrollpane
	 *    attribute: visualUpdate true
	 */
	virtual void setColumnHeader(ViewportPtr columnHeader);
	//-------------------------------------------------------------------------
	/**
	 * Creates a column-header viewport if necessary, sets
	 * its view, and then adds the column-header viewport
	 * to the scrollpane.  For example:
	 * <pre>
	 * JScrollPane scrollpane = new JScrollPane();
	 * scrollpane.setViewportView(myBigComponentToScroll);
	 * scrollpane.setColumnHeaderView(myBigComponentsColumnHeader);
	 * </pre>
	 *
	 * @see #setColumnHeader
	 * @see JViewport#setView
	 *
	 * @param view the component to display as the column header
	 */
	virtual void setColumnHeaderView(AComponentPtr view);
	//-------------------------------------------------------------------------
	/**
	 * Returns the component at the specified corner. The
	 * <code>key</code> value specifying the corner is one of:
	 * <ul>
	 * <li>LOWER_LEFT_CORNER
	 * <li>LOWER_RIGHT_CORNER
	 * <li>UPPER_LEFT_CORNER
	 * <li>UPPER_RIGHT_CORNER
	 * <li>LOWER_LEADING_CORNER
	 * <li>LOWER_TRAILING_CORNER
	 * <li>UPPER_LEADING_CORNER
	 * <li>UPPER_TRAILING_CORNER
	 * </ul>
	 *
	 * @param key one of the values as shown above
	 * @return the corner component (which may be <code>null</code>)
	 *         identified by the given key, or <code>null</code>
	 *         if the key is invalid
	 * @see #setCorner
	 */
	virtual AComponentPtr getCorner(Area loc);
	//-------------------------------------------------------------------------
	/**
	 * Adds a child that will appear in one of the scroll panes
	 * corners, if there's room.   For example with both scrollbars
	 * showing (on the right and bottom edges of the scrollpane)
	 * the lower left corner component will be shown in the space
	 * between ends of the two scrollbars. Legal values for
	 * the <b>key</b> are:
	 * <p>
	 * Although "corner" doesn't match any beans property
	 * signature, <code>PropertyChange</code> events are generated with the
	 * property name set to the corner key.
	 *
	 * @param key identifies which corner the component will appear in
	 * @param corner one of the following components:
	 * <ul>
	 * <li>lowerLeft
	 * <li>lowerRight
	 * <li>upperLeft
	 * <li>upperRight
	 * </ul>
	 * @exception IllegalArgumentException if corner key is invalid
	 */
	virtual void setCorner(Area loc, AComponentPtr corner);
	//-------------------------------------------------------------------------
	/**
	 * Returns a string representation of this <code>ScrollPane</code>.
	 * This method
	 * is intended to be used only for debugging purposes, and the
	 * content and format of the returned string may vary between
	 * implementations. The returned string may be empty but may not
	 * be <code>null</code>.
	 *
	 * @return  a string representation.
	 */
	virtual std::string paramString() const;
	//-------------------------------------------------------------------------
	virtual void resetViewPort();
	//-------------------------------------------------------------------------
	/**
	 * Removes the old viewport (if there is one); forces the
	 * viewPosition of the new viewport to be in the +x,+y quadrant;
	 * syncs up the row and column headers (if there are any) with the
	 * new viewport; and finally syncs the scrollbars and
	 * headers with the new viewport.
	 * <p>
	 * Most applications will find it more convenient to use
	 * <code>setViewportView</code>
	 * to add a viewport and a view to the scrollpane.
	 * @param viewport
	 */
	virtual void setViewport(ViewportPtr viewport);
}; // ScrollPane
}}} // namespace(s)

#endif /* SAMBAG_SCROLLPANE_H */
