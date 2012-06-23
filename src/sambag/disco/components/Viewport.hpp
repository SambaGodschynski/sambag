/*
 * Viewport.hpp
 *
 *  Created on: Sat Jun 23 19:29:57 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_VIEWPORT_H
#define SAMBAG_VIEWPORT_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include "IBorder.hpp"
#include "Forward.hpp"
namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Viewport.
  */
class Viewport : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Viewport> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
	//-------------------------------------------------------------------------
	enum ScrollMode {};
protected:
	//-------------------------------------------------------------------------
	Viewport();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Viewport)
protected:
	//-------------------------------------------------------------------------
	/**
	 * Sets the Viewport's one child, which can be null.
	 * @param child
	 * @param index
	 */
	virtual void addImpl(AComponentPtr child, int index);
	//-------------------------------------------------------------------------
	/**
	 * Computes the parameters for a blit where the backing store image
	 * currently contains oldLoc in the upper left hand corner and we'
	 * re scrolling to newLoc.
	 * @param dx
	 * @param dy
	 * @param blitFrom
	 * @param blitTo
	 * @param blitSize
	 * @param blitPaint
	 * @return
	 */
	virtual bool computeBlit(int dx, int dy, const Point2D & blitFrom,
			const Point2D & blitTo, const Dimension & blitSize,
			const Rectangle & blitPaint);
	//-------------------------------------------------------------------------
	/**
	 * Subclassers can override this to install a different layout
	 * manager (or null) in the constructor.
	 * @return
	 */
	virtual ALayoutManagerPtr createLayoutManager();
	//-------------------------------------------------------------------------
	/**
	 * Notifies listeners of a property change.
	 * @param propertyName
	 * @param oldValue
	 * @param newValue
	 */
	template <typename T>
	void firePropertyChange(const std::string &propertyName,
			const T &oldValue, const T &newValue) const;
	//-------------------------------------------------------------------------
	virtual void fireStateChanged() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns true if scroll mode is a BACKINGSTORE_SCROLL_MODE
	 * to cause painting to originate from Viewport, or one of its ancestors.
	 * @return
	 */
	virtual bool isPaintingOrigin() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns a string representation of this Viewport.
	 * @return
	 */
	virtual std::string paramString() const;
public:
	//-------------------------------------------------------------------------
	/**
	 * Returns the size of the visible part of the view in view coordinates.
	 * @return
	 */
	virtual Dimension getExtentSize() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the insets (border) dimensions as (0,0,0,0), since borders
	 * are not supported on a Viewport.
	 * @return
	 */
	virtual Insets getInsets() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns an Insets object containing this Viewports inset values.
	 * @param insets
	 * @return
	 */
	virtual Insets getInsets(Insets insets) const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the current scrolling mode.
	 * @return
	 */
	virtual ScrollMode getScrollMode() const;
	//-------------------------------------------------------------------------
	virtual AComponentPtr getView() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the view coordinates that appear in the upper left hand
	 * corner of the viewport, or 0,0 if there's no view.
	 * @return
	 */
	virtual Point2D getViewPosition() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns a rectangle whose origin is getViewPosition and size
	 * is getExtentSize.
	 * @return
	 */
	virtual Rectangle getViewRect() const;
	//-------------------------------------------------------------------------
	/**
	 * If the view's size hasn't been explicitly set, return the preferred
	 * size, otherwise return the view's current size.
	 * @return
	 */
	virtual Dimension getViewSize() const;
	//-------------------------------------------------------------------------
	/**
	 * Depending on whether the backingStore is enabled, either paint the image
	 * through the backing store or paint just the recently exposed part,
	 * using the backing store to "blit" the remainder.
	 * @param cn
	 */
	virtual void paint(IDrawContext::Ptr cn);
	//-------------------------------------------------------------------------
	/**
	 * Removes the Viewports one child.
	 * @param child
	 */
	virtual void remove(AComponentPtr child);
	//-------------------------------------------------------------------------
	/**
	 * Always repaint in the parents coordinate system to make sure only
	 * one paint is performed by the RedrawManager.
	 * @param tm
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	virtual void repaint(int x, int y, int w, int h);
	//-------------------------------------------------------------------------
	/**
	 * Sets the bounds of this viewport.
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	virtual void reshape(int x, int y, int w, int h);
	//-------------------------------------------------------------------------
	/**
	 * Scrolls the view so that Rectangle within the view becomes visible.
	 * @param contentRect
	 */
	virtual void scrollRectToVisible(const Rectangle & contentRect);
	//-------------------------------------------------------------------------
	/**
	 * @param border
	 */
	virtual void setBorder(IBorder::Ptr border);
	//-------------------------------------------------------------------------
	/**
	 * Sets the size of the visible part of the view using view coordinates.
	 * @param newExtent
	 */
	virtual void setExtentSize(const Dimension & newExtent);
	//-------------------------------------------------------------------------
	/**
	 * Used to control the method of scrolling the viewport contents.
	 * @param mode
	 */
	virtual void setScrollMode(ScrollMode mode);
	//-------------------------------------------------------------------------
	/**
	 * Sets the Viewport's one child (view), which can be null.
	 * @param view
	 */
	virtual void setView(AComponentPtr view);
	//-------------------------------------------------------------------------
	/**
	 * Sets the view coordinates that appear in the upper left hand corner of
	 * the viewport, does nothing if there's no view.
	 * @param p
	 */
	virtual void setViewPosition(const Point2D & p);
	//-------------------------------------------------------------------------
	/**
	 * Sets the size of the view.
	 * @param newSize
	 */
	virtual void setViewSize(const Dimension & newSize);
	//-------------------------------------------------------------------------
	/**
	 * Converts a size in pixel coordinates to view coordinates.
	 * @param size
	 * @return
	 */
	virtual Dimension toViewCoordinates(const Dimension & size) const;
	//-------------------------------------------------------------------------
	/**
	 * Converts a Point2D in pixel coordinates to view coordinates.
	 * @param p
	 * @return
	 */
	virtual Point2D toViewCoordinates(Point2D p) const;
}; // Viewport
}}} // namespace(s)

#endif /* SAMBAG_VIEWPORT_H */
