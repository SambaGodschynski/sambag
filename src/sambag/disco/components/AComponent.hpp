/*
 * AComponent.hpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#ifndef SAMBAG_COMPONENT_HPP_
#define SAMBAG_COMPONENT_HPP_

#include <boost/shared_ptr.hpp>
#include <sambag/com/events/Events.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/Font.hpp>
#include <sambag/disco/IDrawable.hpp>
#include "ActionMap.hpp"
#include "IBorder.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>
#include <sambag/com/Thread.hpp>
#include <sambag/com/events/PropertyChanged.hpp>

/**
 *  +++
 *  ActionMap
 *  Border
 *  KeyStroke
 *  KeyEvent
 *  MouseEvent
 *  ComponentUI
 *  LayoutManager
 *  FocusListener
 *  ---
 *  FocusTraversalPolicy
 *  TransferHandler
 *  ToolTip
 *  HierarchyBoundsListener
 *  HierarchyListener
 *  PropertyChangeEvent
 *  ComponentEvent
 *  Image
 *  DropTarget
 */

namespace sambag {
namespace disco {
namespace components {
using namespace sambag::com::events;
//=============================================================================
// Forwards
//=============================================================================
//-----------------------------------------------------------------------------
class AContainer;
typedef boost::shared_ptr<AContainer> AContainerPtr;
//=============================================================================
/**
 * @class AComponent
 * A component is an object having a graphical representation that can be
 * displayed on the screen and that can interact with the user.
 */
class AComponent: public IDrawable,
		public EventSender<PropertyChanged> {
	//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AComponent> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<AComponent> WPtr;
	//-------------------------------------------------------------------------
	typedef sambag::com::Mutex Lock;
protected:
	//-------------------------------------------------------------------------
	/*
	 * setted during cretate() by subclasses
	 */
	WPtr self;
	//-------------------------------------------------------------------------
	AComponent();
	//-------------------------------------------------------------------------
	/**
	 * The parent of the object. It may be null.
	 * for top-level components.
	 */
	AContainerPtr parent;
	//-------------------------------------------------------------------------
	/**
	 * The bounds of this Component
	 */
	Rectangle bounds;
	//-------------------------------------------------------------------------
	/**
	 * The foreground color for this component.
	 * foreground can be null.
	 * @see getForeground
	 * @see setForeground
	 */
	ColorRGBA foreground;
	//-------------------------------------------------------------------------
	/**
	 * The background color for this component.
	 * background can be null.
	 * @see getBackground
	 * @see setBackground
	 */
	ColorRGBA background;
	/**
	 * True when the object should ignore all repaint events.
	 * @see setIgnoreRepaint
	 * @see getIgnoreRepaint
	 */
	sambag::com::ArithmeticWrapper<bool> ignoreRepaint;
	//-------------------------------------------------------------------------
	/**
	 * True when the object is visible. An object that is not
	 * visible is not drawn on the screen.
	 * @see isVisible
	 * @see setVisible
	 */
	sambag::com::ArithmeticWrapper<bool, true> visible;
	//-------------------------------------------------------------------------
	/**
	 * True when the object is enabled. An object that is not
	 * enabled does not interact with the user.
	 * @see isEnabled
	 * @see setEnabled
	 */
	sambag::com::ArithmeticWrapper<bool, true> enabled;
private:
	//-------------------------------------------------------------------------
	/**
	 * True when the object is valid. An invalid object needs to
	 * be layed out. This flag is set to false when the object
	 * size is changed.
	 * @see isValid
	 * @see validate
	 * @see invalidate
	 */
	sambag::com::ArithmeticWrapper<bool> valid;
	//-------------------------------------------------------------------------
	/**
	 * A component's name.
	 * @see getName
	 * @see setName(String)
	 */
	std::string name;
	//-------------------------------------------------------------------------
	/**
	 * A bool to determine whether the name has
	 * been set explicitly. <code>nameExplicitlySet</code> will
	 * be false if the name has not been set and
	 * true if it has.
	 * @see getName
	 * @see setName(String)
	 */
	sambag::com::ArithmeticWrapper<bool> nameExplicitlySet;
	//-------------------------------------------------------------------------
	/**
	 * Indicates whether this Component can be focused.
	 * @see setFocusable
	 * @see isFocusable
	 */
	sambag::com::ArithmeticWrapper<bool, true> focusable;
	//-------------------------------------------------------------------------
	/**
	 * The locking object for component-tree and layout operations.
	 * @see getTreeLock
	 */
	Lock treeLock;
	//-------------------------------------------------------------------------
	/*
	 * In some cases using "this" as an object to synchronize by
	 * can lead to a deadlock if client code also uses synchronization
	 * by a component object. For every such situation revealed we should
	 * consider possibility of replacing "this" with the package private.
	 */
	Lock objectLock;
	//-------------------------------------------------------------------------
	Lock & getObjectLock();
	//-------------------------------------------------------------------------
	/**
	 * Minimum size.
	 * (This field perhaps should have been transient).
	 */
	Dimension minSize;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not setMinimumSize has been invoked with a non-null value.
	 */
	sambag::com::ArithmeticWrapper<bool> minSizeSet;
	//-------------------------------------------------------------------------
	/**
	 * Preferred size.
	 * (This field perhaps should have been transient).
	 */
	Dimension prefSize;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not setPreferredSize has been invoked with a non-null value.
	 */
	sambag::com::ArithmeticWrapper<bool> prefSizeSet;
	//-------------------------------------------------------------------------
	/**
	 * Maximum size
	 */
	Dimension maxSize;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not setMaximumSize has been invoked with a non-null value.
	 */
	sambag::com::ArithmeticWrapper<bool> maxSizeSet;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool> isPacked;
protected:
	//-------------------------------------------------------------------------
	void enable();
	//-------------------------------------------------------------------------
	void disable();
public:
	//-------------------------------------------------------------------------
	/**
	 * Ease-of-use constant for <code>getAlignmentY()</code>.
	 * Specifies an alignment to the top of the component.
	 * @see     getAlignmentY
	 */
	static const float TOP_ALIGNMENT = 0.0f;
	//-------------------------------------------------------------------------
	/**
	 * Ease-of-use constant for <code>getAlignmentY</code> and
	 * <code>getAlignmentX</code>. Specifies an alignment to
	 * the center of the component
	 * @see     getAlignmentX
	 * @see     getAlignmentY
	 */
	static const float CENTER_ALIGNMENT = 0.5f;
	//-------------------------------------------------------------------------
	/**
	 * Ease-of-use constant for <code>getAlignmentY</code>.
	 * Specifies an alignment to the bottom of the component.
	 * @see     getAlignmentY
	 */
	static const float BOTTOM_ALIGNMENT = 1.0f;
	//-------------------------------------------------------------------------
	/**
	 * Ease-of-use constant for <code>getAlignmentX</code>.
	 * Specifies an alignment to the left side of the component.
	 * @see     getAlignmentX
	 */
	static const float LEFT_ALIGNMENT = 0.0f;
	//-------------------------------------------------------------------------
	/**
	 * Ease-of-use constant for <code>getAlignmentX</code>.
	 * Specifies an alignment to the right side of the component.
	 * @see     getAlignmentX
	 */
	static const float RIGHT_ALIGNMENT = 1.0f;
public:
	//-------------------------------------------------------------------------
	virtual Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	virtual ~AComponent();
	//-------------------------------------------------------------------------
	virtual Rectangle getBounds() const {
		return getBoundingBox();
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const;
	//-------------------------------------------------------------------------
	virtual std::string toString() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the alignment along the x axis. This specifies how the component
	 * would like to be aligned relative to other components. The value should
	 * be a number between 0 and 1 where 0 represents alignment along the origin,
	 *  1 is aligned the furthest away from the origin, 0.5 is centered, etc.
	 *
	 */
	virtual Coordinate getAlignmentX() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the alignment along the y axis. This specifies how the component
	 * would like to be aligned relative to other components. The value should
	 * be a number between 0 and 1 where 0 represents alignment along the origin,
	 *  1 is aligned the furthest away from the origin, 0.5 is centered, etc.
	 *
	 */
	virtual Coordinate getAlignmentY() const;
	//-------------------------------------------------------------------------
	/**
	 * Determines if this component or one of its immediate subcomponents
	 * contains the (x, y) location, and if so, returns the containing component.
	 * @param x
	 * @param y
	 * @return
	 */
	virtual Ptr getComponentAt(const Coordinate & x, const Coordinate & y) const;
	//-------------------------------------------------------------------------
	/**
	 * @return the current height of this component.
	 */
	virtual Coordinate getHeight() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the location of this component in the form of a point
	 * specifying the component's top-left corner.
	 */
	virtual Point2D getLocation() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the maximum size of this component.
	 */
	virtual Dimension getMaximumSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the mininimum size of this component.
	 */
	virtual Dimension getMinimumSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the name of the component.
	 */
	virtual std::string getName() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the parent of this component.
	 */
	virtual AContainerPtr getParent() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the preferred size of this component.
	 */
	virtual Dimension getPreferredSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the size of this component.
	 */
	virtual Dimension getSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the current width of this component.
	 */
	virtual Coordinate getWidth() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the current x coordinate of the components origin.
	 */
	virtual Coordinate getX() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the current y coordinate of the components origin.
	 */
	virtual Coordinate getY() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the ActionMap used to determine what Action to fire for
	 * particular KeyStroke binding.
	 * @return
	 */
	virtual ActionMap::Ptr getActionMap() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the autoscrolls property.
	 */
	virtual bool getAutoscrolls() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the baseline. The baseline is measured from the top of the
	 * component. This method is primarily meant for LayoutManagers to align
	 * components along their baseline. A return value less than 0 indicates this
	 * component does not have a reasonable baseline and that LayoutManagers
	 * should not align this component on its baseline.
	 * This method calls into the ComponentUI method of the same name.
	 * If this component does not have a ComponentUI -1 will be returned.
	 * If a value >= 0 is returned, then the component has a valid baseline
	 * for any size >= the minimum size and getBaselineResizeBehavior can be
	 * used to determine how the baseline changes with size.
	 * @param width
	 * @param height
	 * @return the baseline or < 0 indicating there is no reasonable baseline
	 */
	virtual Coordinate getBaseLine(const Coordinate &width,
			const Coordinate &height);
	//-------------------------------------------------------------------------
	/**
	 * Returns the border of this component or null if no border is currently set.
	 * @return
	 */
	virtual IBorder::Ptr getBorder() const;
	//-------------------------------------------------------------------------
	/**
	 * Requests that this Component get the input focus, and that this
	 * Component's top-level ancestor become the focused Window.
	 */
	virtual void grabFocus();
	//-------------------------------------------------------------------------
	/**
	 * Creates a graphics context for this component. This method will
	 * return null if this component is currently not
	 * displayable.
	 * @return a graphics context for this component, or null
	 */
	virtual IDrawContext::Ptr getIDrawContext() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the Background color
	 */
	virtual ColorRGBA getBackground() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the Foreground color
	 */
	virtual ColorRGBA getForeground() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the Lock (mutex) object
	 */
	virtual Lock & getTreeLock();
	//-------------------------------------------------------------------------
	/**
	 * @return true if this AComponent is the focus owner.
	 */
	virtual bool hasFocus() const;
	//-------------------------------------------------------------------------
	/**
	 * Invalidates this component. This component and all parents above
	 * it are marked as needing to be laid out. This method can be called
	 * often, so it needs to execute quickly.
	 */
	virtual void invalidate();
	//-------------------------------------------------------------------------
	/**
	 * @return true if this component is displayable.
	 */
	virtual bool isDisplayable() const;
	//-------------------------------------------------------------------------
	/**
	 * @return true if component is enabled
	 */
	virtual bool isEnabled() const;
	//-------------------------------------------------------------------------
	/**
	 *
	 * @return whether this AComponent can be focused.
	 */
	virtual bool isFocusable() const;
	//-------------------------------------------------------------------------
	/**
	 * @return true if this AComponent is the focus owner.
	 */
	virtual bool isFocusOwner() const;
	//-------------------------------------------------------------------------
	/**
	 * @return true if this component is completely opaque,
	 * returns false by default.
	 */
	virtual bool isOpaque() const;
	//-------------------------------------------------------------------------
	/**
	 * Determines whether this component is showing on screen. This means
	 * that the component must be visible, and it must be in a container
	 * that is visible and showing.
	 * <p>
	 * <strong>Note:</strong> sometimes there is no way to detect whether the
	 * {@code Component} is actually visible to the user.  This can happen when:
	 * <ul>
	 * <li>the component has been added to a visible {@code ScrollPane} but
	 * the {@code Component} is not currently in the scroll pane's view port.
	 * <li>the {@code Component} is obscured by another {@code Component} or
	 * {@code Container}.
	 * </ul>
	 * @return <code>true</code> if the component is showing,
	 *          <code>false</code> otherwise
	 */
	 virtual bool isShowing() const;
	 //-------------------------------------------------------------------------
	 /**
	 * Determines whether this component is valid. A component is valid when it
	 * is correctly sized and positioned within its parent container and all
	 * its children are also valid. Components are invalidated when they are
	 * first shown on the screen.
	 * @return true if the component is valid, false otherwise
	 */
	virtual bool isValid() const;
	//-------------------------------------------------------------------------
	/**
	 * @return whether this component should be visible when its parent is visible.
	 */
	virtual bool isVisible() const;
	//-------------------------------------------------------------------------
	/**
	 * @return whether maximum size is set
	 */
	virtual bool isMaximumSizeSet() const;
	//-------------------------------------------------------------------------
	/**
	 * @return whether minimum size is set
	 */
	virtual bool isMinimumSizeSet() const;
	//-------------------------------------------------------------------------
	/**
	 * @return whether Background color is set
	 */
	virtual bool isBackgroundSet() const;
	//-------------------------------------------------------------------------
	/**
	 * @return whether Foreground color is set
	 */
	virtual bool isForegroundSet() const;
	//-------------------------------------------------------------------------
	/**
	 * Draw this component
	 * @param
	 */
	virtual void draw(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	/**
	 * Draws this component and all of its subcomponents.
	 * @param context
	 */
	virtual void drawAll(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	/**
	 *  Redraws this component.
	 */
	virtual void redraw();
protected:
	//-------------------------------------------------------------------------
	/**
	 * Draw this component's border
	 * @param
	 */
	virtual void drawBorder(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	/**
	 * Calls the UI delegate's paint method, if the UI delegate is non-null.
	 * We pass the delegate a copy of the Graphics object to protect the rest
	 * of the paint code from irrevocable changes (for example, Graphics.translate).
	 * If you override this in a subclass you should not make permanent changes
	 * to the passed in Graphics. For example, you should not alter the clip
	 *  Rectangle or modify the transform. If you need to do these operations
	 *  you may find it easier to create a new Graphics from the passed in
	 *  Graphics and manipulate it. Further, if you do not invoker super's
	 *  implementation you must honor the opaque property, that is if this
	 *  component is opaque, you must completely fill in the background
	 *  in a non-opaque color. If you do not honor the opaque property you
	 *  will likely see visual artifacts.
	 * The passed in Graphics object might have a transform other than the
	 * identify transform installed on it. In this case, you might get unexpected
	 * results if you cumulatively apply another transform.
	 * @param
	 */
	virtual void drawComponent(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	/**
	 * Determines whether this component will be displayed on the screen.
	 * @return <code>true</code> if the component and all of its ancestors
	 *          until a toplevel window or null parent are visible,
	 *          <code>false</code> otherwise
	 */
	virtual bool isRecursivelyVisible() const;
public:
	//-------------------------------------------------------------------------
	/**
	 * Prompts the layout manager to lay out this component.
	 * This method is primarily intended to operate on instances of Container.
	 */
	virtual void doLayout() {
	}
	//-------------------------------------------------------------------------
	/**
	 *  Requests that this AComponent get the input focus, and that this
	 *  AComponent's top-level ancestor become the focused Window.
	 */
	virtual void requestFocus();
	//-------------------------------------------------------------------------
	/**
	 * Moves and resizes this component.
	 * @param x
	 * @param y
	 * @param width
	 * @param height
	 */
	virtual void setBounds(const Coordinate &x, const Coordinate & y,
			const Coordinate & width, const Coordinate & height) {
		setBounds(Rectangle(Point2D(x, y), width, height));
	}
	//-------------------------------------------------------------------------
	/**
	 * Moves and resizes this component.
	 * @param r
	 */
	virtual void setBounds(const Rectangle &r);
	//-------------------------------------------------------------------------
	/**
	 * Enables or disables this component, depending on the value of
	 * the parameter b.
	 * @param b
	 */
	virtual void setEnabled(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Sets the focusable state of this AComponent to the specified value.
	 * @param b
	 */
	virtual void setFocusable(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Sets whether or not paint messages received from the operating system
	 * should be ignored.
	 * @param b
	 */
	virtual void setIgnoreRepaint(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Moves this component to a new location.
	 * @param x
	 * @param y
	 */
	virtual void setLocation(const Coordinate &x, const Coordinate &y) {
		setLocation(Point2D(x, y));
	}
	//-------------------------------------------------------------------------
	/**
	 * Moves this component to a new location.
	 * @param p
	 */
	virtual void setLocation(const Point2D &p);
	//-------------------------------------------------------------------------
	/**
	 * Sets the name of the component to the specified string.
	 * @param name
	 */
	virtual void setName(const std::string &name);
	//-------------------------------------------------------------------------
	/**
	 * Resizes this component so that it has width d.width and height d.height.
	 * @param d
	 */
	virtual void setSize(const Dimension &d);
	//-------------------------------------------------------------------------
	/**
	 * Resizes this component so that it has width d.width and height d.height.
	 * @param d
	 */
	virtual void setSize(const Coordinate &width, const Coordinate &height) {
		setSize(Dimension(width, height));
	}
	//-------------------------------------------------------------------------
	/**
	 * Shows or hides this component depending on the value of parameter b.
	 * @param b
	 */
	virtual void setVisible(bool b);
	//-------------------------------------------------------------------------
	/**
	 * Sets the ActionMap.
	 * @param am
	 */
	virtual void setActionMap(ActionMap::Ptr am) const;
	//-------------------------------------------------------------------------
	/**
	 * Sets the autoscrolls property.
	 * @param b
	 */
	virtual void setAutoscrolls(bool b) const;
	//-------------------------------------------------------------------------
	/**
	 * Sets the border of this component.
	 * @param br
	 */
	virtual void setBorder(IBorder::Ptr br) const;
	//-------------------------------------------------------------------------
	/**
	 * Sets the background color of this component.
	 * The background color affects each component differently and the parts
	 * of the component that are affected by the background color.
	 * @param c
	 */
	virtual void setBackground(const ColorRGBA &c);
	//-------------------------------------------------------------------------
	/**
	 * Sets the foreground color of this component.
	 * @param c
	 */
	virtual void setForeground(const ColorRGBA &c);
	//-------------------------------------------------------------------------
	/**
	 * Transfers the focus to the next component,
	 * as though this AComponent were the focus owner.
	 */
	virtual void transferFocus();
	//-------------------------------------------------------------------------
	/**
	 * Transfers the focus to the previous component,
	 * as though this AComponent were the focus owner.
	 */
	virtual void transferFocusBackward();
	//-------------------------------------------------------------------------
	/**
	 * Ensures that this component has a valid layout. This method is primarily
	 * intended to operate on instances of Container.
	 */
	virtual void validate();
	//-------------------------------------------------------------------------
	/**
	 * Updates the container.
	 * @param cn
	 */
	virtual void update(IDrawContext::Ptr cn);
};

}
}
}

#endif /* COMPONENT_HPP_ */
