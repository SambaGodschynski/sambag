/*
 * AComponent.hpp
 *
 *  Created on: May 15, 2012
 *      Author: samba
 */

#ifndef SAMBAG_COMPONENT_HPP_
#define SAMBAG_COMPONENT_HPP_

#include <boost/shared_ptr.hpp>
#include <sambag/com/Events.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/Font.hpp>
#include <sambag/disco/IDrawable.hpp>
#include "ActionMap.hpp"
#include "IBorder.hpp"
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

namespace sambag { namespace disco { namespace components {
//=============================================================================
/**
 * @class AComponent
 * A component is an object having a graphical representation that can be
 * displayed on the screen and that can interact with the user.
 */
class AComponent : public IDrawable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AComponent> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<AComponent> WPtr;
protected:
	//-------------------------------------------------------------------------
	/*
	 * setted during cretate() by subclasses
	 */
	WPtr self;
	//-------------------------------------------------------------------------
	AComponent();
private:
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
	virtual Ptr getParent() const;
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
	virtual Coordinate getBaseLine(const Coordinate &width, const Coordinate &height);
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
	IDrawContext::Ptr getIDrawContext() const;
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
	 *
	 * @return whether this component is showing on screen.
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
	//-------------------------------------------------------------------------
	virtual Dimension isMinimumSizeSet() const;
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
	//-------------------------------------------------------------------------
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
public:
	//-------------------------------------------------------------------------
	/**
	 * Prompts the layout manager to lay out this component.
	 * This method is primarily intended to operate on instances of Container.
	 */
	virtual void doLayout() {}
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
	virtual void setBounds(const Coordinate &x,
			const Coordinate & y,
			const Coordinate & width,
			const Coordinate & height)
	{
		setBounds(Rectangle(Point2D(x,y), width, height));
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
		setLocation(Point2D(x,y));
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

}}}

#endif /* COMPONENT_HPP_ */
