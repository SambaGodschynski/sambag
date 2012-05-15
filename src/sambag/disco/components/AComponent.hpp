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
 * The base class for all Disco components except top-level containers.
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
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
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
	//-------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------
	/**
	 * @return true if component is enabled
	 */
	virtual bool isEnabled() const;
	//--------------------------------------------------------------------------
	/**
	 *
	 * @return whether this AComponent can be focused.
	 */
	virtual bool isFocusable() const;
	//--------------------------------------------------------------------------
	/**
	 * @return true if this AComponent is the focus owner.
	 */
	virtual bool isFocusOwner() const;
	//--------------------------------------------------------------------------
	/**
	 * @return true if this component is completely opaque,
	 * returns false by default.
	 */
	virtual bool isOpaque() const;
	//--------------------------------------------------------------------------
	/**
	 *
	 * @return whether this component is showing on screen.
	 */
	virtual bool isShowing() const;
	//--------------------------------------------------------------------------
	/**
	 * Determines whether this component is valid. A component is valid when it
	 * is correctly sized and positioned within its parent container and all
	 * its children are also valid. Components are invalidated when they are
	 * first shown on the screen.
	 * @return true if the component is valid, false otherwise
	 */
	virtual bool isValid() const;
	//--------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------
	/**
	 * Draw this component
	 * @param
	 */
	virtual void draw(IDrawContext::Ptr context);
	//--------------------------------------------------------------------------
	/**
	 * Draws this component and all of its subcomponents.
	 * @param context
	 */
	virtual void drawAll(IDrawContext::Ptr context);
	//--------------------------------------------------------------------------
	/**
	 *  Redraws this component.
	 */
	virtual void redraw();
	//--------------------------------------------------------------------------;
	/**
	 * Prompts the layout manager to lay out this component.
	 * This method is primarily intended to operate on instances of Container.
	 */
	virtual void doLayout() {}
	//--------------------------------------------------------------------------
	/**
	 *  Requests that this AComponent get the input focus, and that this
	 *  AComponent's top-level ancestor become the focused Window.
	 */
	virtual void requestFocus();
	//--------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------
	/**
	 * Moves and resizes this component.
	 * @param r
	 */
	virtual void setBounds(const Rectangle &r);
	//--------------------------------------------------------------------------
	/**
	 * Enables or disables this component, depending on the value of
	 * the parameter b.
	 * @param b
	 */
	virtual void setEnabled(bool b);
	//--------------------------------------------------------------------------
	/**
	 * Sets the focusable state of this AComponent to the specified value.
	 * @param b
	 */
	virtual void setFocusable(bool b);
	//--------------------------------------------------------------------------
	/**
	 * Sets whether or not paint messages received from the operating system
	 * should be ignored.
	 * @param b
	 */
	virtual void setIgnoreRepaint(bool b);
	//--------------------------------------------------------------------------
	/**
	 * Moves this component to a new location.
	 * @param x
	 * @param y
	 */
	void setLocation(const Coordinate &x, const Coordinate &y) {
		setLocation(Point2D(x,y));
	}
	//--------------------------------------------------------------------------
	/**
	 * Moves this component to a new location.
	 * @param p
	 */
	void setLocation(const Point2D &p);
	//--------------------------------------------------------------------------
	/**
	 * Sets the name of the component to the specified string.
	 * @param name
	 */
	void setName(const std::string &name);
	//--------------------------------------------------------------------------
	/**
	 * Resizes this component so that it has width d.width and height d.height.
	 * @param d
	 */
	void setSize(const Dimension &d);
	//--------------------------------------------------------------------------
	/**
	 * Resizes this component so that it has width d.width and height d.height.
	 * @param d
	 */
	void setSize(const Coordinate &width, const Coordinate &height) {
		setSize(Dimension(width, height));
	}
	//--------------------------------------------------------------------------
	/**
	 * Shows or hides this component depending on the value of parameter b.
	 * @param b
	 */
	virtual void setVisible(bool b);
	//--------------------------------------------------------------------------
	/**
	 * Transfers the focus to the next component,
	 * as though this AComponent were the focus owner.
	 */
	void transferFocus();
	//--------------------------------------------------------------------------
	/**
	 * Transfers the focus to the previous component,
	 * as though this AComponent were the focus owner.
	 */
	void transferFocusBackward();
	//--------------------------------------------------------------------------
	/**
	 * Ensures that this component has a valid layout. This method is primarily
	 * intended to operate on instances of Container.
	 */
	void validate();
	//--------------------------------------------------------------------------
	void update(IDrawContext::Ptr cn);
};

}}}

#endif /* COMPONENT_HPP_ */
