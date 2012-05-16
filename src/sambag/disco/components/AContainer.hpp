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
class AContainer : public AComponent {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AContainer> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<AComponent::Ptr> Components;
protected:
	//-------------------------------------------------------------------------
	AContainer();
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	Components components;
public:
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
			const Coordinate & y) const
	{
		return getComponentAt(Point2D(x,y));
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
	virtual Dimension getMaximumSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the mininimum size of this component.
	 */
	//-------------------------------------------------------------------------
	virtual Dimension getMinimumSize() const;
	//-------------------------------------------------------------------------
	/**
	 * @return the preferred size of this component.
	 */
	virtual Dimension getPreferredSize() const;
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
}}} // namespace(s)

#endif /* SAMBAG_ACONTAINER_H */
