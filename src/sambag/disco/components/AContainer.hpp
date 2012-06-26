/*
 * AContainer.hpp
 *
 *  Created on: Wed May 16 12:00:29 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ACONTAINER_H
#define SAMBAG_ACONTAINER_H

#include <boost/shared_ptr.hpp>
#include "AComponent.hpp"
#include "ALayoutManager.hpp"
#include <vector>
#include "events/ContainerEvent.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
 * @class AContainer.
 * A generic container object is a component that can contain other Components.
 * Components added to a container are tracked in a list.
 * The order of the list will define the components' front-to-back stacking
 * order within the container. If no index is specified when adding a component
 * to a container, it will be added to the end of the list (and hence to the
 * bottom of the stacking order).
 */
class AContainer: public AComponent,
		public EventSender<events::ContainerEvent> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AContainer> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<AComponent::Ptr> Components;
	//-------------------------------------------------------------------------
	static const bool descendUnconditionallyWhenValidating = false;
protected:
	//-------------------------------------------------------------------------
	AContainer();
	//-------------------------------------------------------------------------
	void adjustDescendants(int num);
	//-------------------------------------------------------------------------
	// Should only be called while holding tree lock
	void adjustDecendantsOnParent(int num);
	//-------------------------------------------------------------------------
	/**
	 * installs specific component ui on sub components.
	 * @param c
	 */
	virtual void installLookAndFeel (ui::ALookAndFeelPtr laf);
private:
	//-------------------------------------------------------------------------
	void retargetMouseEvent(AComponentPtr c, events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void dispatchAddEvents(AComponent::Ptr comp);
	//--------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> descendantsCount;
	//-------------------------------------------------------------------------
	/**
	 * Checks that the component
	 * isn't supposed to be added into itself.
	 */
	//-------------------------------------------------------------------------
	void checkAddToSelf(AComponent::Ptr c) const;
	//-------------------------------------------------------------------------
	Components components;
	//-------------------------------------------------------------------------
	Dimension getMaximumSizeAlt();
	//-------------------------------------------------------------------------
	Dimension getMinimumSizeAlt();
	//-------------------------------------------------------------------------
	Dimension getPreferredSizeAlt();
	//-------------------------------------------------------------------------
	AComponentPtr lastMouseTarget;
	//-------------------------------------------------------------------------
	void trackMouseEnterEvents(AComponentPtr target,
			const events::MouseEvent &ev);
protected:
	//--------------------------------------------------------------------------
	void addComponent(AComponent::Ptr comp, int index = -1);
	//-------------------------------------------------------------------------
	virtual void processMouseEvent(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	virtual std::string parameterString() const;
	//-------------------------------------------------------------------------
	/**
	 * Layout manager for this container.
	 * @see #doLayout
	 * @see #setLayout
	 * @see #getLayout
	 */
	ALayoutManager::Ptr layoutMgr;
	//-------------------------------------------------------------------------
	/**
	 * @override
	 */
	virtual int __countHierarchyMembers_() const;
	//-------------------------------------------------------------------------
	/**
	 * @override
	 */
	virtual int __dispatchHierarchyEvents_(events::HierarchyEvent::Type id,
			AComponentPtr changed, AContainerPtr changedParent,
			long changeFlags);
	//-------------------------------------------------------------------------
	/**
	 * Invalidates the parent of this component if any.
	 * This method MUST BE invoked under the TreeLock.
	 */
	virtual void invalidateParent();
	//-------------------------------------------------------------------------
	/**
	 * Recursively descends the container tree and invalidates all
	 * contained components.
	 */
	virtual void invalidateTree();
	//-------------------------------------------------------------------------
	virtual void drawChildren(IDrawContext::Ptr cn);
	//-------------------------------------------------------------------------
	enum ObscuredState {
		NOT_OBSCURED, COMPLETELY_OBSCURED, PARTIALLY_OBSCURED
	};
	//-------------------------------------------------------------------------
	/**
	 * Returns whether or not the region of the specified component is
	 * obscured by a sibling.
	 *
	 * @return NOT_OBSCURED if non of the siblings above the Component obscure
	 *         it, COMPLETELY_OBSCURED if one of the siblings completely
	 *         obscures the Component or PARTIALLY_OBSCURED if the Comonent is
	 *         only partially obscured.
	 */
	ObscuredState
	getObscuredState(int compIndex, const Rectangle &rect) const;
	//-------------------------------------------------------------------------
	/**
	 * If the specified rectangle is completely obscured by any of this
	 * component's opaque children then returns true.  Only direct children
	 * are considered, more distant descendants are ignored.  A
	 * <code>JComponent</code> is opaque if
	 * <code>JComponent.isOpaque()</code> returns true, other lightweight
	 * components are always considered transparent, and heavyweight components
	 * are always considered opaque.
	 *
	 * @param x  x value of specified rectangle
	 * @param y  y value of specified rectangle
	 * @param width  width of specified rectangle
	 * @param height height of specified rectangle
	 * @return true if the specified rectangle is obscured by an opaque child
	 */
	virtual bool rectangleIsObscured(const Rectangle &r);
public:
	//-------------------------------------------------------------------------
	/**
	 * writes all components into stream
	 * @param the stream
	 */
	virtual void printComponentTree(std::ostream &ss) const;
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		Ptr s = boost::shared_dynamic_cast<AContainer>(self.lock());
		return s;
	}
	//-------------------------------------------------------------------------
	virtual ~AContainer();
	//-------------------------------------------------------------------------
	/**
	 * Adds the specified component to this container. Note: If a component has
	 * been added to a container that has been displayed, validate must be
	 * called on that container to display the new component. If multiple
	 * components are being added, you can improve efficiency by calling
	 * validate only once, after all the components have been added.
	 * @param comp
	 * @param index
	 * @return the component argument
	 */
	virtual AComponent::Ptr add(AComponent::Ptr comp, int index = -1);
	//-------------------------------------------------------------------------
	template<class LayoutManager, class Constraint>
	AComponent::Ptr add(AComponent::Ptr comp, const Constraint &c,
			int index = -1);
	//-------------------------------------------------------------------------
	/**
	 *  Causes this container to lay out its components.
	 */
	virtual void doLayout();
	//-------------------------------------------------------------------------
	/**
	 * @param x
	 * @param y
	 * @return the visible child component that contains the specified position.
	 */
	virtual AComponent::Ptr getComponentAt(const Coordinate &x,
			const Coordinate & y) const {
		return getComponentAt(Point2D(x, y));
	}
	//-------------------------------------------------------------------------
	/**
	 * @param p
	 * @return visible child component that contains the specified position.
	 */
	virtual AComponent::Ptr getComponentAt(const Point2D &p) const;
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
	 * @param n
	 * @note This method should be called under tree lock.
	 * @return the nth component in this container.
	 */
	virtual AComponent::Ptr getComponent(size_t n) const;
	//-------------------------------------------------------------------------
	/**
	 *  Gets the number of components in this panel.
	 * @return
	 */
	virtual size_t getComponentCount() const;
	//-------------------------------------------------------------------------
	/**
	 * @return all the components in this container.
	 */
	virtual Components & getComponents();
	//-------------------------------------------------------------------------
	/**
	 * @return all the components in this container.
	 */
	virtual const Components & getComponents() const;
	//-------------------------------------------------------------------------
	/**
	 * Returns the z-order index of the component inside the container.
	 * The higher a component is in the z-order hierarchy, the lower
	 * its index.  The component with the lowest z-order index is
	 * painted last, above all other child components.
	 *
	 * @param comp the component being queried
	 * @return  the z-order index of the component; otherwise
	 *          returns -1 if the component is <code>null</code>
	 *          or doesn't belong to the container
	 */
	int getComponentZOrder(AComponent::Ptr comp) const;
	//-------------------------------------------------------------------------
	/**
	 * Determines the insets of this container, which indicate the size of the
	 * container's border.
	 * @return
	 */
	virtual Insets getInsets() const;
	//-------------------------------------------------------------------------
	virtual ALayoutManager::Ptr getLayout() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the maximum size of this component.
	 */
	virtual Dimension getMaximumSize();
	//-------------------------------------------------------------------------
	/**
	 * @return the mininimum size of this component.
	 */
	//-------------------------------------------------------------------------
	virtual Dimension getMinimumSize();
	//-------------------------------------------------------------------------
	/**
	 * @return the preferred size of this component.
	 */
	virtual Dimension getPreferredSize();
	//-------------------------------------------------------------------------
	/**
	 * Checks if the component is contained in the component hierarchy of
	 * this container.
	 * @param c
	 * @return
	 */
	virtual bool isAncestorOf(AComponent::Ptr c) const;
	//-------------------------------------------------------------------------
	/**
	 * Indicates if this container is a <i>validate root</i>.
	 * <p>
	 * Layout-related changes, such as bounds of the validate root descendants,
	 * do not affect the layout of the validate root parent. This peculiarity
	 * enables the {@code invalidate()} method to stop invalidating the
	 * component hierarchy when the method encounters a validate root. However,
	 * to preserve backward compatibility this new optimized behavior is
	 * enabled only when the {@code java.awt.smartInvalidate} system property
	 * value is set to {@code true}.
	 * <p>
	 * If a component hierarchy contains validate roots and the new optimized
	 * {@code invalidate()} behavior is enabled, the {@code validate()} method
	 * must be invoked on the validate root of a previously invalidated
	 * component to restore the validity of the hierarchy later. Otherwise,
	 * calling the {@code validate()} method on the top-level container (such
	 * as a {@code Frame} object) should be used to restore the validity of the
	 * component hierarchy.
	 * <p>
	 * @return whether this container is a validate root
	 * @see #invalidate
	 * @see AComponent#invalidate
	 */
	virtual bool isValidateRoot() const;
	//-------------------------------------------------------------------------
	/**
	 * Invalidates this component. This component and all parents above
	 * it are marked as needing to be laid out. This method can be called
	 * often, so it needs to execute quickly.
	 * @override
	 */
	virtual void invalidate();
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
	virtual void drawComponents(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified component from this container.
	 * @param comp
	 */
	virtual void remove(AComponent::Ptr comp);
	//-------------------------------------------------------------------------
	/**
	 * Removes the component, specified by index, from this container.
	 * @param comp
	 */
	virtual void remove(size_t index);
	//-------------------------------------------------------------------------
	/**
	 * Removes all the components from this container.
	 */
	virtual void removeAll();
	//-------------------------------------------------------------------------
	virtual void setLayout(ALayoutManager::Ptr mgr);
	//-------------------------------------------------------------------------
	AComponentPtr findComponentAt(const Point2D &p,
			bool includeSelf = true);
	//-------------------------------------------------------------------------
	AComponentPtr findComponentAt(const Coordinate &x, const Coordinate &y,
			bool includeSelf = true)
	{
		return findComponentAt(Point2D(x,y), includeSelf);
	}
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
	//-------------------------------------------------------------------------
	/**
	 * Recursively descends the container tree and recomputes the layout for any
	 * subtrees marked as needing it (those marked as invalid).
	 */
	virtual void validateTree();
}; // AContainer
//=============================================================================
template<class LayoutManager, class Constraint>
AComponent::Ptr AContainer::add(AComponent::Ptr comp, const Constraint &c,
		int index) {
	if (!comp)
		return AComponent::Ptr();
	SAMBAG_BEGIN_SYNCHRONIZED(getTreeLock())
		addComponent(comp, index);
		if (layoutMgr)
			layoutMgr->addLayoutComponent<LayoutManager> (comp, c);
	SAMBAG_END_SYNCHRONIZED
	dispatchAddEvents(comp);
	return comp;
}
}}} // namespace(s)

#endif /* SAMBAG_ACONTAINER_H */
