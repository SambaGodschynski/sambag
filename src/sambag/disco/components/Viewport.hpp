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
#include <sambag/disco/components/Timer.hpp>

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
	enum ScrollMode {
		SIMPLE_SCROLL_MODE,
		BLIT_SCROLL_MODE,
		BACKINGSTORE_SCROLL_MODE
	};
protected:
	//-------------------------------------------------------------------------
	/**
	 * True when the viewport dimensions have been determined.
	 * The default is false.
	 */
	sambag::com::ArithmeticWrapper<bool> isViewSizeSet;
	//-------------------------------------------------------------------------
	/**
	 * The last <code>viewPosition</code> that we've painted, so we know how
	 * much of the backing store image is valid.
	 */
	Point2D lastPaintPosition;
	//-------------------------------------------------------------------------
	/** The view image used for a backing store. */
	IImageSurface::Ptr backingStoreImage;
	//-------------------------------------------------------------------------
	Viewport();
	//-------------------------------------------------------------------------
	ScrollMode scrollMode;
private:
	//-------------------------------------------------------------------------
	/**
	 * This is set to true in <code>setViewPosition</code>
	 * if doing a window blit and the viewport is obscured.
	 */
	bool repaintAll;
	//-------------------------------------------------------------------------
	/**
	 * This is set to true in paint, if <code>repaintAll</code>
	 * is true and the clip rectangle does not match the bounds.
	 * If true, and scrolling happens the
	 * repaint manager is not cleared which then allows for the repaint
	 * previously invoked to succeed.
	 */
	bool waitingForRepaint;
	//-------------------------------------------------------------------------
	/**
	 * Instead of directly invoking repaint, a <code>Timer</code>
	 * is started and when it fires, repaint is invoked.
	 */
	Timer::Ptr repaintTimer;
	//-------------------------------------------------------------------------
	/**
	 * Set to true in paintView when paint is invoked.
	 */
	bool inBlitPaint;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not a valid view has been installed.
	 */
	bool hasHadValidView;
	//-------------------------------------------------------------------------
	/**
	 * The <code>scrollUnderway</code> flag is used for components like
	 * <code>List</code>.  When the downarrow key is pressed on a
	 * <code>List</code> and the selected
	 * cell is the last in the list, the <code>scrollpane</code> autoscrolls.
	 * Here, the old selected cell needs repainting and so we need
	 * a flag to make the viewport do the optimized painting
	 * only when there is an explicit call to
	 * <code>setViewPosition(Point)</code>.
	 * When <code>setBounds</code> is called through other routes,
	 * the flag is off and the view repaints normally.  Another approach
	 * would be to remove this from the <code>JViewport</code>
	 * class and have the <code>JList</code> manage this case by using
	 * <code>setBackingStoreEnabled</code>.  The default is
	 * <code>false</code>.
	 */
	sambag::com::ArithmeticWrapper<bool> scrollUnderway;
private:
	//-------------------------------------------------------------------------
	IDrawContext::Ptr getBackingStoreContext(IDrawContext::Ptr cn);
	//-------------------------------------------------------------------------
	/**
	 * Ascends the <code>Viewport</code>'s parents stopping when
	 * a component is found that returns
	 * <code>true</code> to <code>isValidateRoot</code>.
	 * If all the <code>Component</code>'s  parents are visible,
	 * <code>validate</code> will then be invoked on it. The
	 * <code>RepaintManager</code> is then invoked with
	 * <code>removeInvalidComponent</code>. This
	 * is the synchronous version of a <code>revalidate</code>.
	 */
	void validateView();
	//-------------------------------------------------------------------------
	/**  Used by the scrollRectToVisible method to determine the
	 *  proper direction and amount to move by. The integer variables are named
	 *  width, but this method is applicable to height also. The code assumes that
	 *  parentWidth/childWidth are positive and childAt can be negative.
	 */
	Coordinate positionAdjustment(const Coordinate &parentWidth,
			const Coordinate &childWidth, const Coordinate &childAt);
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
	virtual void addComponent(AComponentPtr child, int index = -1);
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
