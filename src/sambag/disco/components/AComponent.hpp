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
#include "events/MouseEvent.hpp"
#include "events/HierarchyEvent.hpp"
#include "Forward.hpp"
#include "IBorder.hpp"
#include <boost/unordered_map.hpp>
#include <sambag/com/AbstractType.hpp>
#include <sstream>


/**
 *  +++
 *  ActionMap
 *  Border
 *  KeyStroke
 *  KeyEvent
 *  MouseEvent
 *  AComponentUI
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

#define SAMBAG_STD_STATIC_COMPONENT_CREATOR(sambag_macro_class_name)		\
	Ptr getPtr() const {													\
		return boost::shared_dynamic_cast<sambag_macro_class_name>(AComponent::getPtr());\
	}																		\
	static Ptr create() {													\
		Ptr res(new sambag_macro_class_name());								\
		res->self = res;													\
		return res;															\
	}


namespace sambag { namespace disco { namespace components {
using namespace sambag::com::events;
//=============================================================================
/**
 * @class AComponent
 * A component is an object having a graphical representation that can be
 * displayed on the screen and that can interact with the user.
 */
class AComponent: public IDrawable,
		public EventSender<PropertyChanged>,
		public EventSender<events::MouseEvent>,
		public EventSender<events::HierarchyEvent> {
//=============================================================================
friend class RedrawManager;
friend class RootPane;
friend class AContainer;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AComponent> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<AComponent> WPtr;
	//-------------------------------------------------------------------------
	typedef sambag::com::Mutex Lock;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<std::string, AbstractType::Ptr> PropertyMap;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_NAME;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_FOREGROUND;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_BACKGROUND;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_PREFERREDSIZE;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_MAXIMUMSIZE;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_MINIMUMSIZE;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_FOCUSABLE;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_UI;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_BORDER;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_ENABLED;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_CLIENTPROPERTY;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_SIZE;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_FONT;
protected:
	//-------------------------------------------------------------------------
	Font font;
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
	//-------------------------------------------------------------------------
	/**
	 * Whether or not autoscroll has been enabled.
	 */
	sambag::com::ArithmeticWrapper<bool, true> autoScrolls;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, false> uiSettedByUser;
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr ui;
	//-------------------------------------------------------------------------
	IBorder::Ptr border;
	//-------------------------------------------------------------------------
	virtual std::string parameterString() const;
private:
	//-------------------------------------------------------------------------
	PropertyMap propertyMap;
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
	mutable Lock treeLock;
	//-------------------------------------------------------------------------
	/*
	 * In some cases using "this" as an object to synchronize by
	 * can lead to a deadlock if client code also uses synchronization
	 * by a component object. For every such situation revealed we should
	 * consider possibility of replacing "this" with the package private.
	 */
	mutable Lock objectLock;
	//-------------------------------------------------------------------------
	Lock & getObjectLock() const;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not setMinimumSize has been invoked with a non-null value.
	 */
	sambag::com::ArithmeticWrapper<bool> minSizeSet;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not setPreferredSize has been invoked with a non-null value.
	 */
	sambag::com::ArithmeticWrapper<bool> prefSizeSet;
	//-------------------------------------------------------------------------
	/**
	 * Whether or not setMaximumSize has been invoked with a non-null value.
	 */
	sambag::com::ArithmeticWrapper<bool> maxSizeSet;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool> isPacked;
	//-------------------------------------------------------------------------
	void enable();
	//-------------------------------------------------------------------------
	void disable();
	//-------------------------------------------------------------------------
	void show();
	//-------------------------------------------------------------------------
	void hide();
protected:
	//-------------------------------------------------------------------------
	/**
	 * Gets ComponentUI by look and feel. Every concrete Component needs
	 * to impl. this method because the look and feel component relation
	 * is type dependent.
	 *
	 * @param c
	 */
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const {
		return ui::AComponentUIPtr();
	}
	//-------------------------------------------------------------------------
	/**
	 * installs specific component ui. Every Component has to impl. its own version.
	 *
	 * @param c
	 */
	virtual void installUI(ui::AComponentUIPtr cui);
	//-------------------------------------------------------------------------
	virtual void installLookAndFeel(ui::ALookAndFeelPtr laf);
	//-------------------------------------------------------------------------
	/**
	 * Maximum size
	 */
	Dimension maxSize;
	//-------------------------------------------------------------------------
	/**
	 * Minimum size.
	 * (This field perhaps should have been transient).
	 */
	Dimension minSize;
	//-------------------------------------------------------------------------
	/**
	 * Preferred size.
	 * (This field perhaps should have been transient).
	 */
	Dimension prefSize;
	//-------------------------------------------------------------------------
	bool isTreeLocked();
	//-------------------------------------------------------------------------
	void checkTreeLock();
	//-------------------------------------------------------------------------
	template<class Event, class _EventSender>
	void dispatchEvent(const Event &ev, _EventSender *sender) {
		sender->EventSender<Event>::notifyListeners(sender, ev);
	}
	//-------------------------------------------------------------------------
	template <typename ValueType>
	void firePropertyChanged(const std::string &name,
			const ValueType &oldValue, const ValueType &newValue)
	{
		dispatchEvent(
			sambag::com::events::PropertyChanged(name, oldValue, newValue),
			this
		);
	}
	//-------------------------------------------------------------------------
	virtual void clearMostRecentFocusOwnerOnHide();
	//-------------------------------------------------------------------------
	/**
	 * Transfers the focus to the next component, as though this Component were
	 * the focus owner.
	 * @see       #requestFocus()
	 */
	virtual void transferFocus(bool clearOnFailure = false);
	//-------------------------------------------------------------------------
	virtual void redrawParentIfNeeded(const Rectangle &r);
	//-------------------------------------------------------------------------
	/**
	 * Invalidates the parent of this component if any.
	 * This method MUST BE invoked under the TreeLock.
	 */
	virtual void invalidateParent();
	//-------------------------------------------------------------------------
	/**
	 * Invoked from putClientProperty.  This is provided for subclasses.
	 * @param key
	 * @param oldValue
	 * @param newValue
	 */
	virtual void clientPropertyChanged(const std::string &key,
			AbstractType::Ptr oldValue, AbstractType::Ptr newValue) {}

protected: // TODO: remove mangeling
	//-------------------------------------------------------------------------
	virtual void processMouseEvent(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	/**
	 * @return
	 */
	virtual int __countHierarchyMembers_() const {
		return 1;
	}
	//-------------------------------------------------------------------------
	void __updateParent_(AContainerPtr cn) {
		parent = cn;
	}
	//-------------------------------------------------------------------------
	virtual int __dispatchHierarchyEvents_(events::HierarchyEvent::Type id,
			AComponentPtr changed, AContainerPtr changedParent,
			long changeFlags);
	//-------------------------------------------------------------------------
	/**
	 * Invalidates the component unless it is already invalid.
	 */
	virtual void __invalidateIfValid_();
public: /* END should be protected  */
	//-------------------------------------------------------------------------
	/**
	 * Fetches the root container somewhere higher up in the component
	 * tree that contains this component.
	 */
	virtual AContainerPtr getRootContainer() const;
	//-------------------------------------------------------------------------
	bool isUiSettedByUser() const {
		return uiSettedByUser;
	}
	//-------------------------------------------------------------------------
	/**
	 * installs specific component ui.
	 *
	 * @param c
	 */
	virtual void setUserUI (ui::AComponentUIPtr cui);
	//-------------------------------------------------------------------------
	virtual void setFont(const Font &font);
	//-------------------------------------------------------------------------
	virtual const Font & getFont() const;
	//-------------------------------------------------------------------------
	virtual Font & getFont();
	//-------------------------------------------------------------------------
	/**
	 * removes component ui.
	 *
	 * @param c
	 */
	virtual void removeUserUI ();
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
	//-----------------------------------------------------------------------------
	virtual bool contains(Point2D p) const;
	//-----------------------------------------------------------------------------
	/**
	 * Checks whether this component "contains" the specified point,
	 * where the point's <i>x</i> and <i>y</i> coordinates are defined
	 * to be relative to the coordinate system of this component.
	 * @param     p     the point
	 * @see       #getComponentAt(Point)
	 */
	virtual bool contains(const Coordinate &x, const Coordinate &y) const {
		return contains(Point2D(x, y));
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	virtual ~AComponent();
	//-------------------------------------------------------------------------
	virtual const Rectangle & getBounds() const;
	//-------------------------------------------------------------------------
	/**
	 * implements IDrawable
	 * @return
	 */
	virtual Rectangle getBoundingBox() const {
		return getBounds();
	}
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
	virtual Ptr getComponentAt(const Coordinate & x, const Coordinate & y) const {
		return getComponentAt(Point2D(x, y));
	}
	//-------------------------------------------------------------------------
	virtual Ptr getComponentAt(const Point2D &p) const;
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
	virtual const Point2D & getLocation() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the maximum size of this component.
	 * @note call can change minSize
	 */
	virtual Dimension getMaximumSize();
	//-------------------------------------------------------------------------
	/**
	 * @return the mininimum size of this component.
	 * @note call can change minSize
	 */
	virtual Dimension getMinimumSize();
	//-------------------------------------------------------------------------
	/**
	 * @return the name of the component.
	 */
	virtual const std::string & getName() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the parent of this component.
	 */
	virtual AContainerPtr getParent() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the preferred size of this component.
	 * @note call can change prefSize
	 */
	virtual Dimension getPreferredSize();
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
	virtual const Coordinate & getX() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the current y coordinate of the components origin.
	 */
	virtual const Coordinate & getY() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the ActionMap used to determine what Action to fire for
	 * particular KeyStroke binding.
	 * @return
	 */
	virtual ActionMap::Ptr getActionMap() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the top-level ancestor of this component,
	 * or <code>null</code> if this component has not
	 * been added to any container.
	 *
	 * @return the top-level <code>Container</code> that this component is in,
	 *          or <code>null</code>
	 */
	virtual RootPanePtr getTopLevelRootPane() const;
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
	 * This method calls into the AComponentUI method of the same name.
	 * If this component does not have a AComponentUI -1 will be returned.
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
	virtual IDrawContext::Ptr getDrawContext() const;
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
	virtual Lock & getTreeLock() const;
	//-------------------------------------------------------------------------
	/**
	 * @return whether or not paint messages received.
	 */
	virtual bool getIgnoreRepaint() const;
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
	 * @note checks parents recursively onto top whether displayable
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
	 * Returns true if this component is completely opaque, returns
	 * false by default.
	 * <p>
	 * An opaque component paints every pixel within its
	 * rectangular region. A non-opaque component paints only some of
	 * its pixels, allowing the pixels underneath it to "show through".
	 * A component that does not fully paint its pixels therefore
	 * provides a degree of transparency.
	 * <p>
	 * Subclasses that guarantee to always completely paint their
	 * contents should override this method and return true.
	 *
	 * @return true if this component is completely opaque
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
	 * Returns true if the preferred size has been set to a
	 * non-<code>null</code> value otherwise returns false.
	 *
	 * @return true if <code>setPreferredSize</code> has been invoked
	 *         with a non-null value.
	 */
	virtual bool isPreferredSizeSet() const;
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
	virtual void drawChildren(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	/**
	 *  Redraws this component.
	 */
	virtual void redraw();
	//-------------------------------------------------------------------------
	/**
	 *  Redraws this component.
	 */
	virtual void redraw(const Rectangle &r);
protected:
	//-------------------------------------------------------------------------
	virtual AContainerPtr getContainer() const {
		return getParent();
	}
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
	//-------------------------------------------------------------------------
	/**
	 * Returns the graphics object used to paint this component.
	 * If <code>DebugGraphics</code> is turned on we create a new
	 * <code>DebugGraphics</code> object if necessary.
	 * Otherwise we just configure the
	 * specified graphics object's foreground and font.
	 */
	virtual IDrawContext::Ptr getComponentDrawContext(IDrawContext::Ptr cn) const;
public:
	//-------------------------------------------------------------------------
	/**
	 * converts component location to screen location.
	 * @param p
	 * @return
	 */
	virtual Point2D getLocationOnScreen(const Point2D &p) const;
	//-------------------------------------------------------------------------
	/**
	 * converts screen location to component location.
	 * @param p
	 * @return
	 */
	virtual Point2D getLocationOnComponent(const Point2D &p) const;
	//-------------------------------------------------------------------------
	AContainerPtr getValidateRoot() const;
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
	 * Revalidates the component hierarchy up to the nearest validate root.
	 * <p>
	 * This method first invalidates the component hierarchy starting from this
	 * component up to the nearest validate root. Afterwards, the component
	 * hierarchy is validated starting from the nearest validate root.
	 * <p>
	 * This is a convenience method supposed to help application developers
	 * avoid looking for validate roots manually. Basically, it's equivalent to
	 * first calling the {@link #invalidate()} method on this component, and
	 * then calling the {@link #validate()} method on the nearest validate
	 * root.
	 *
	 * @see Container#isValidateRoot
	 */
	virtual void revalidate();
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
	virtual void setBorder(IBorder::Ptr br);
	//------------------------------------------------------------------------
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
	 * Sets the preferred size of this component to a constant
	 * value.  Subsequent calls to <code>getPreferredSize</code> will always
	 * return this value.  Setting the preferred size to <code>null</code>
	 * restores the default behavior.
	 *
	 * @param preferredSize The new preferred size, or null
	 * @see #getPreferredSize
	 * @see #isPreferredSizeSet
	 */
	virtual void setPreferredSize(const Dimension &preferredSize);
	//-------------------------------------------------------------------------
	/**
	 * Sets the minimum size of this component to a constant
	 * value.  Subsequent calls to <code>getMinimumSize</code> will always
	 * return this value.  Setting the minimum size to <code>null</code>
	 * restores the default behavior.
	 * @param minimumSize the new minimum size of this component
	 * @see #getMinimumSize
	 * @see #isMinimumSizeSet
	 */
	virtual void setMinimumSize(const Dimension &s);
	//-------------------------------------------------------------------------
	/**
	 * Sets the maximum size of this component to a constant
	 * value.  Subsequent calls to <code>getMaximumSize</code> will always
	 * return this value.  Setting the maximum size to <code>null</code>
	 * restores the default behavior.
	 * @param maximumSize a <code>Dimension</code> containing the
	 *          desired maximum allowable size
	 * @see #getMaximumSize
	 * @see #isMaximumSizeSet
	 */
	virtual void setMaximumSize(const Dimension &s);
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
	virtual void update(IDrawContext::Ptr cn) {
		draw(cn);
	}
	//-------------------------------------------------------------------------
	const PropertyMap & getClientProperties() const {
		return propertyMap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the value of the property with the specified key.  Only
	 * properties added with <code>putClientProperty</code> will return
	 * a non-<code>null</code> value.
	 *
	 * @param key the being queried
	 * @return the value of this property or <code>null</code>
	 * @see #putClientProperty
	 */
	AbstractType::Ptr getClientProperty(const std::string &key) const;
	//-------------------------------------------------------------------------
	/**
	 * Adds an arbitrary key/value "client property" to this component.
	 * <p>
	 * The <code>get/putClientProperty</code> methods provide access to
	 * a small per-instance hashtable. Callers can use get/putClientProperty
	 * to annotate components that were created by another module.
	 * For example, a
	 * layout manager might store per child constraints this way. For example:
	 * <pre>
	 * componentA.putClientProperty("to the left of", componentB);
	 * </pre>
	 * If value is <code>null</code> this method will remove the property.
	 * Changes to client properties are reported with
	 * <code>PropertyChange</code> events.
	 * The name of the property (for the sake of PropertyChange
	 * events) is <code>key.toString()</code>.
	 * <p>
	 * @param key the new client property key
	 * @param value the new client property value; if <code>null</code>
	 *          this method will remove the property
	 * @see #getClientProperty
	 * @see #addPropertyChangeListener
	 */
	void putClientProperty(const std::string &key, AbstractType::Ptr value);
};
}}}

#endif /* COMPONENT_HPP_ */
